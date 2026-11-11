/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danicamp <danicamp@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 13:45:57 by danicamp          #+#    #+#             */
/*   Updated: 2026/11/11 14:57:47 by danicamp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int		stop_simulation(t_coder *coder)
{
	int	status;
	int	coders_done;

	status = 0;
	coders_done = 0;
	pthread_mutex_lock(&coder->program->end_lock);
	coders_done = coder->program->coders_done;
	if (coder->compiles_done == coder->program->config.number_of_compiles_required
		&& !coder->done)
		coder->program->coders_done++;
	if (coder->compiles_done == coder->program->config.number_of_compiles_required)
			coder->done = 1;
	if (coders_done == coder->program->config.number_of_coders)
	{	
		coder->program->simulation_over = 1;
		status = coder->program->simulation_over;
	}
	pthread_mutex_unlock(&coder->program->end_lock);
	return (status);
}

static void	is_compiling(t_coder *coder)
{
	pthread_mutex_lock(&coder->state_lock);
	coder->compiles_done ++;
	coder->last_compile_start = current_time(coder->program);
	pthread_mutex_unlock(&coder->state_lock);
	print_log(coder, "is compiling");
	usleep(coder->program->config.time_to_compile * 1000);
}

static void	is_debbuging(t_coder *coder)
{
	print_log(coder, "is debbuging");
	usleep(coder->program->config.time_to_debug * 1000);
}


static void	is_refactoring(t_coder *coder)
{
	print_log(coder, "is refactoring");
	usleep(coder->program->config.time_to_refactor * 1000);	
}

static void	get_dongle(t_dongle *dongle, t_sim *program)
{
	pthread_mutex_lock(&dongle->lock);
	while (dongle->in_use || dongle->available_at > current_time(program))
	{
		pthread_mutex_unlock(&dongle->lock);
		usleep(1000);
		pthread_mutex_lock(&dongle->lock);
	}
	dongle->in_use = 1;
	pthread_mutex_unlock(&dongle->lock);
}

static void	free_dongle(t_dongle *dongle, t_sim *program)
{
	long	cooldown;
	
	cooldown = program->config.dongle_cooldown;
	pthread_mutex_lock(&dongle->lock);
	if (dongle->in_use)
	{
		dongle->available_at = current_time(program) + cooldown;
		dongle->in_use = 0;
	}
	pthread_mutex_unlock(&dongle->lock);
}

static void	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	
	if (coder->left_dongle->id < coder->right_dongle->id)
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	else
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	get_dongle(first, coder->program);
	print_log(coder, "has taken a dongle");
	get_dongle(second, coder->program);
	print_log(coder, "has taken a dongle");
}


static void	leave_dongles(t_coder *coder)
{	
	free_dongle(coder->left_dongle, coder->program);
	free_dongle(coder->right_dongle, coder->program);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	
	coder = (t_coder *)arg;
	while (!stop_simulation(coder))
	{
		if (!coder->done)
		{
			take_dongles(coder);
			is_compiling(coder);
			is_debbuging(coder);
			is_refactoring(coder);
			leave_dongles(coder);
		}
	}
	return (NULL);
}
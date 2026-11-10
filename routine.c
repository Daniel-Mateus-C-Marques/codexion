/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danicamp <danicamp@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 13:45:57 by danicamp          #+#    #+#             */
/*   Updated: 2026/11/10 17:04:39 by danicamp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int		stop_simulation(t_coder *coder)
{
	int	status;
	
	status = 0;
	if (coder->compiles_done >= coder->program->config.number_of_compiles_required)
	{	
		pthread_mutex_lock(&coder->program->end_lock);
		coder->program->simulation_over = 1;
		status = coder->program->simulation_over;
		pthread_mutex_unlock(&coder->program->end_lock);
	}
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

static int	get_dongle(t_dongle *dongle, t_sim *program)
{
	pthread_mutex_lock(&dongle->lock);
	if (dongle->in_use || dongle->available_at > current_time(program))
	{	
		pthread_mutex_unlock(&dongle->lock);
		return (0);
	}
	dongle->in_use = 1;
	pthread_mutex_unlock(&dongle->lock);
	return (1);
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
		pthread_cond_broadcast(&dongle->cond);
	}
	pthread_mutex_unlock(&dongle->lock);
}

static int	take_dongles(t_coder *coder)
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
	if (!get_dongle(first, coder->program))
		return (0);
	if (!get_dongle(second, coder->program))
	{
		free_dongle(first, coder->program);
		return (0);
	}
	print_log(coder, "has taken a dongle");
	print_log(coder, "has taken a dongle");
	return (1);
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
		if (take_dongles(coder))
		{
			is_compiling(coder);
			is_debbuging(coder);
			is_refactoring(coder);
			leave_dongles(coder);
		}
	}
	return (NULL);
}
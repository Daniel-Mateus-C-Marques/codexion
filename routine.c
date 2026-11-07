/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danicamp <danicamp@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 13:45:57 by danicamp          #+#    #+#             */
/*   Updated: 2026/11/07 14:21:47 by danicamp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int		stop_simulation(t_sim *program)
{
	int	status;
	
	pthread_mutex_lock(&program->end_lock);
	status = program->simulation_over;
	pthread_mutex_unlock(&program->end_lock);
	return (status);
}

static void	is_compiling(t_coder *coder)
{
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

static void	get_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	while (dongle->in_use)
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	/* aqui o mutex está trancado e a condição é verdadeira */
	dongle->in_use = 1;
	pthread_mutex_unlock(&dongle->lock);
}

static void	take_dongles(t_coder *coder)
{
	get_dongle(coder->left_dongle);
	print_log(coder, "has taken a dongle");
	get_dongle(coder->right_dongle);
	print_log(coder, "has taken a dongle");
}

static void	free_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->in_use = 0;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}

static void	leave_dongles(t_coder *coder)
{
	free_dongle(coder->left_dongle);
	free_dongle(coder->right_dongle);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	int		comp;
	
	coder = (t_coder *)arg;
	comp = 0;
	while (!stop_simulation(coder->program))
	{
		take_dongles(coder);
		is_compiling(coder);
		is_debbuging(coder);
		is_refactoring(coder);
		leave_dongles(coder);
		comp++;
		if (comp == coder->program->config.number_of_compiles_required)
			coder->program->simulation_over = 1;
	}
	return (NULL);
}
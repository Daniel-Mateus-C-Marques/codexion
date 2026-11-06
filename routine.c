/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danicamp <danicamp@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 13:45:57 by danicamp          #+#    #+#             */
/*   Updated: 2026/11/06 16:57:43 by danicamp         ###   ########.fr       */
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
	usleep(coder->program->config.time_to_compile * 1000);
}


static void	is_refactoring(t_coder *coder)
{
	print_log(coder, "is refactoring");
	usleep(coder->program->config.time_to_compile * 1000);	
}


void	*coder_routine(void *arg)
{
	t_coder	*coder;
	int		comp;
	
	coder = (t_coder *)arg;
	comp = 0;
	while (!stop_simulation(coder->program))
	{
		is_compiling(coder);
		is_debbuging(coder);
		is_refactoring(coder);
		comp++;
		if (comp == coder->program->config.number_of_compiles_required)
			coder->program->simulation_over = 1;
	}
	return (NULL);
}
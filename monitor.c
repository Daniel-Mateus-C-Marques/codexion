/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danicamp <danicamp@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 16:50:05 by danicamp          #+#    #+#             */
/*   Updated: 2026/11/16 13:43:22 by danicamp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	burnout_checker(t_coder *coder)
{
	long	deadline;
	
	deadline = 0;
	pthread_mutex_lock(&coder->state_lock);
	deadline = coder->last_compile_start + coder->program->config.time_to_burnout;
	pthread_mutex_unlock(&coder->state_lock);
	if (deadline < current_time(coder->program))
		return (1);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim	*program;
	int		i;
	int		n;

	program = (t_sim *)arg;
	n = program->config.number_of_coders;
	i = 0;
	while (!program->simulation_over)
	{
		while (i < n)
		{
			if (!program->coders[i].done && burnout_checker(&program->coders[i]))
			{
				pthread_mutex_lock(&program->end_lock);
				print_log(&program->coders[i], "burned out");
				program->simulation_over = 1;
				pthread_mutex_unlock(&program->end_lock);
				return (NULL);
			}
			i++;
		}
		i = 0;
		usleep(1000);
	}
	return (NULL);
}
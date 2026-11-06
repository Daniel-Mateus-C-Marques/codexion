/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danicamp <danicamp@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 14:03:03 by danicamp          #+#    #+#             */
/*   Updated: 2026/11/06 11:57:00 by danicamp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	destroy_dongles(t_sim *program, int count)
{
	while (count > 0)
	{
		count--;
		pthread_mutex_destroy(&program->dongles[count].lock);
		pthread_cond_destroy(&program->dongles[count].cond);
	}
}

int	init_dongles(t_sim *program)
{
	int	i;

	i = 0;
	while (i < program->config.number_of_coders)
	{
		program->dongles[i].id = i + 1;
		program->dongles[i].in_use = 0;
		program->dongles[i].available_at = 0;
		if (pthread_mutex_init(&program->dongles[i].lock, NULL))
		{
			destroy_dongles(program, i);
			return (-1);
		}
		if (pthread_cond_init(&program->dongles[i].cond, NULL))
		{
			pthread_mutex_destroy(&program->dongles[i].lock);
			destroy_dongles(program, i);
			return (-1);
		}
		i++;
	}
	return (0);
}
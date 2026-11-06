/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danicamp <danicamp@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 13:44:47 by danicamp          #+#    #+#             */
/*   Updated: 2026/11/06 15:01:07 by danicamp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void start_time(t_sim *program)
{
	program->start_time = get_time_ms();
}

int	start_threads(t_sim *program)
{
	int	i;

	i = 0;
	start_time(program);
	while (i < program->config.number_of_coders)
	{
		if (pthread_create(&program->coders[i].thread, NULL,
				coder_routine, &program->coders[i]) != 0)
			return (1);
		i++;
	}
	i = 0;
	while (i < program->config.number_of_coders)
	{
		pthread_join(program->coders[i].thread, NULL);
		i++;
	}
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danicamp <danicamp@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 13:44:47 by danicamp          #+#    #+#             */
/*   Updated: 2026/11/16 15:51:54 by danicamp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	start_threads(t_sim *program)
{
	int	i;

	i = 0;
	if (pthread_create(&program->monitor_thread, NULL, monitor_routine, program))
		return (1);
	while (i < program->config.number_of_coders)
	{
		if (pthread_create(&program->coders[i].thread, NULL,
				coder_routine, &program->coders[i]))
			return (1);
		i++;
	}
	i = 0;
	while (i < program->config.number_of_coders)
	{
		pthread_join(program->coders[i].thread, NULL);
		i++;
	}
	pthread_join(program->monitor_thread, NULL);
	return (0);
}
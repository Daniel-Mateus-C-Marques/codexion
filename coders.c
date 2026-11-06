/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danicamp <danicamp@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 17:34:15 by danicamp          #+#    #+#             */
/*   Updated: 2026/11/06 11:57:45 by danicamp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	destroy_coder_locks(t_coder *coders, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&coders[i].state_lock);
		i++;
	}
}

static void	set_coder_dongles(t_sim *program, int i)
{
	t_coder	*coder;
	int		n;

	coder = &program->coders[i];
	n = program->config.number_of_coders;
	coder->left_dongle = &program->dongles[i];
	if (n == 1)
		coder->right_dongle = NULL;
	else
		coder->right_dongle = &program->dongles[(i + 1) % n];
}

int	init_coders(t_sim *program)
{
	int	i;

	i = 0;
	while (i < program->config.number_of_coders)
	{
		program->coders[i].id = i + 1;
		program->coders[i].compiles_done = 0;
		program->coders[i].last_compile_start = program->start_time;
		program->coders[i].program = program;
		set_coder_dongles(program, i);
		if (pthread_mutex_init(&program->coders[i].state_lock, NULL) != 0)
		{
			destroy_coder_locks(program->coders, i);
			return (1);
		}
		i++;
	}
	return (0);
}
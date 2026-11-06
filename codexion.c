/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danicamp <danicamp@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 14:32:54 by danicamp          #+#    #+#             */
/*   Updated: 2026/11/06 12:14:35 by danicamp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	assign_dongle(t_sim *program, t_config config)
{
	int	i;
	int n;

	n = config.number_of_coders;
	i = 0;
	while (i < n)
	{
		program->coders[i].right_dongle = &program->dongles[i];
		program->coders[i].left_dongle = &program->dongles[(i - 1 + n) % n];
		program->coders[i].id = i + 1;
		i++;
	}
}

int	init_sim(t_sim *program, t_config config)
{
	program->config = config;
	program->coders = malloc(sizeof(t_coder) * config.number_of_coders);
	if (!program->coders)
		return (-1);
	program->dongles = malloc(sizeof(t_dongle) * config.number_of_coders);
	if (!program->dongles)
	{
		free(program->coders);
		return (-1);
	}
	init_dongles(program);
	init_coders(program);
	return (0);
}

int main(int argc, char **argv)
{
	t_config	config;
	t_sim		program;

	if (argc != 9)
	{
		fprintf(stderr, "Wrong number of arguments!\n");
		return 1;
	}
	config = parser(argv);
	if (!config_checker(config))
	{
		fprintf(stderr, "Wrong argument provided.\n");
		return (1);
	}
	if (init_sim(&program, config))
	{
		fprintf(stderr, "Allocation failed.\n");
		return (1);
	}
	assign_dongle(&program, config);
	debug_print_sim(&program);
	return (0);
}
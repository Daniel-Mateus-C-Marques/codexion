/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danicamp <danicamp@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 14:32:54 by danicamp          #+#    #+#             */
/*   Updated: 2026/09/24 20:31:30 by danicamp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void print_config(t_config config)
{
	printf("coders: %d\n", config.number_of_coders);
	printf("burnout: %d\n", config.time_to_burnout);
	printf("compile: %d\n", config.time_to_compile);
	printf("debug: %d\n", config.time_to_debug);
	printf("refactor: %d\n", config.time_to_refactor);
	printf("c_required: %d\n", config.number_of_compiles_required);
	printf("d_cooldown: %d\n", config.dongle_cooldown);
	printf("scheduler: %d\n", config.scheduler);
}

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
		free(program->dongles);
		return (-1);
	}
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
	print_config(config);
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
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danicamp <danicamp@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 14:30:44 by danicamp          #+#    #+#             */
/*   Updated: 2026/09/23 20:53:16 by danicamp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int number_parser(char *number)
{
	long	nb;
	int		i;

	i = 0;
	if (!number[i])
		return (-1);
	while (number[i])
	{
		if (number[i] < '0' || number[i] > '9')
			return (-1);
		i++;
	}
	nb = ft_atoi(number);
	if (nb > 2147483647 || nb < -2147483648)
		return (-1);
	return ((int)nb);
}

static t_scheduler scheduler_parser(char *scheduler)
{
	int i;

	i = 0;
	while (scheduler[i])
	{
		if (scheduler[i] >= 'A' && scheduler[i] <= 'Z')
			scheduler[i] += 32;
		i++;
	}
	if (!strcmp("fifo", scheduler))
		return (0);
	else if (!strcmp("edf", scheduler))
		return (1);
	else
		return (2);
}

t_config parser(char **argv)
{
	t_config config;

	config.number_of_coders = number_parser(argv[1]);
	config.time_to_burnout = number_parser(argv[2]);
	config.time_to_compile = number_parser(argv[3]);
	config.time_to_debug = number_parser(argv[4]);
	config.time_to_refactor = number_parser(argv[5]);
	config.number_of_compiles_required = number_parser(argv[6]);
	config.dongle_cooldown = number_parser(argv[7]);
	config.scheduler = scheduler_parser(argv[8]);
	return (config);
}

int config_checker(t_config config)
{
	if (config.number_of_coders <= 0)
		return (0);
	if (config.time_to_burnout <= 0)
		return (0);
	if (config.time_to_compile <= 0)
		return (0);
	if (config.time_to_debug <= 0)
		return (0);
	if (config.time_to_refactor <= 0)
		return (0);
	if (config.number_of_compiles_required <= 0)
		return (0);
	if (config.dongle_cooldown < 0)
		return (0);
	if (config.scheduler == 2)
		return (0);
	return (1);
}
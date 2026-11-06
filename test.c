/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danicamp <danicamp@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 12:12:57 by danicamp          #+#    #+#             */
/*   Updated: 2026/11/06 12:13:21 by danicamp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "codexion.h"

static void	print_dongle(t_dongle *dongle)
{
	printf("  dongle %d | in_use=%d | available_at=%ld | addr=%p\n",
		dongle->id, dongle->in_use, dongle->available_at,
		(void *)dongle);
}

static int	dongle_id(t_dongle *dongle)
{
	if (dongle == NULL)
		return (-1);
	return (dongle->id);
}

static void	print_coder(t_coder *coder)
{
	printf("  coder %d | left=%d | right=%d | last_compile_start=%ld",
		coder->id, dongle_id(coder->left_dongle),
		dongle_id(coder->right_dongle), coder->last_compile_start);
	printf(" | compiles_done=%d | sim_ok=%d\n",
		coder->compiles_done, coder->program != NULL);
}

void	debug_print_sim(t_sim *sim)
{
	int	i;
	int	n;

	n = sim->config.number_of_coders;
	printf("=== SIM === start_time=%ld | coders=%d\n", sim->start_time, n);
	printf("--- DONGLES ---\n");
	i = 0;
	while (i < n)
	{
		print_dongle(&sim->dongles[i]);
		i++;
	}
	printf("--- CODERS ---\n");
	i = 0;
	while (i < n)
	{
		print_coder(&sim->coders[i]);
		i++;
	}
	printf("===========\n");
}
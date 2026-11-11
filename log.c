/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danicamp <danicamp@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 14:05:43 by danicamp          #+#    #+#             */
/*   Updated: 2026/11/11 13:49:27 by danicamp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_log(t_coder *coder, char *action)
{
	long	current_time;
	
	current_time = get_time_ms() - coder->program->start_time;
	pthread_mutex_lock(&coder->program->log_lock);
	printf("%ld %d %s\n", current_time, coder->id, action);
	pthread_mutex_unlock(&coder->program->log_lock);
}
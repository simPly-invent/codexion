/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_second.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:57:57 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/01 16:04:23 by mobenais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"
#include <pthread.h>
#include <stdio.h>

bool	get_simu_state(t_simulation *state)
{
	bool	result;

	pthread_mutex_lock(&state->mutex);
	result = state->simu_state;
	pthread_mutex_unlock(&state->mutex);
	return (result);
}

void	secure_log(t_character *chara, char *message, long timestamp)
{
	pthread_mutex_lock(&chara->state->mutex);
	printf("%ld %d %s\n", timestamp, chara->coder->id, message);
	pthread_mutex_unlock(&chara->state->mutex);
}

void	secure_message(t_character *chara, char *message)
{
	pthread_mutex_lock(&chara->state->mutex);
	printf("%s", message);
	pthread_mutex_unlock(&chara->state->mutex);
}

long	convert_time_stamp_coder(t_coder *coder)
{
	long			result;
	struct timeval	res;

	pthread_mutex_lock(&coder->mutex);
	gettimeofday(&res, NULL);
	result = (res.tv_sec - coder->last_time_compile.tv_sec) * 1000
		+ (res.tv_usec - coder->last_time_compile.tv_usec) / 1000;
	pthread_mutex_unlock(&coder->mutex);
	return (result);
}

long	get_timestamp_ms(t_character *chara)
{
	long			result;
	struct timeval	res;

	gettimeofday(&res, NULL);
	result = (res.tv_sec - chara->state->start.tv_sec) * 1000 + (res.tv_usec
			- chara->state->start.tv_usec) / 1000;
	return (result);
}

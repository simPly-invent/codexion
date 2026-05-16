/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_second.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:57:57 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/12 14:46:16 by mobenais         ###   ########.fr       */
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

void	secure_log(t_character *chara, char *message)
{
	struct timeval	now;
	long			ts;

	pthread_mutex_lock(&chara->state->mutex);
	if (!chara->state->simu_state)
	{
		pthread_mutex_unlock(&chara->state->mutex);
		return ;
	}
	gettimeofday(&now, NULL);
	ts = (now.tv_sec - chara->state->start.tv_sec) * 1000
		+ (now.tv_usec - chara->state->start.tv_usec) / 1000;
	printf("%ld %d %s\n", ts, chara->coder->id, message);
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
	struct timeval	start;

	pthread_mutex_lock(&chara->state->mutex);
	start = chara->state->start;
	pthread_mutex_unlock(&chara->state->mutex);
	gettimeofday(&res, NULL);
	result = (res.tv_sec - start.tv_sec) * 1000 + (res.tv_usec - start.tv_usec)
		/ 1000;
	return (result);
}

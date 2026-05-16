/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:56:39 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/16 17:09:26 by mobenais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

void	coder_refactor(t_character *chara)
{
	if (!get_simu_state(chara->state))
		return ;
	secure_log(chara, "is refactoring");
	check_state_session(chara->coder->refractor, chara);
}

void	coder_debug(t_character *chara)
{
	if (!get_simu_state(chara->state))
		return ;
	secure_log(chara, "is debugging");
	check_state_session(chara->coder->debug, chara);
}

void	mark_coder_done(t_character *chara)
{
	pthread_mutex_lock(&chara->coder->mutex);
	chara->coder->last_time_compile.tv_sec = LONG_MAX / 1000;
	chara->coder->last_time_compile.tv_usec = 0;
	pthread_mutex_unlock(&chara->coder->mutex);
	pthread_mutex_lock(&chara->state->mutex);
	chara->state->coders_done++;
	pthread_mutex_unlock(&chara->state->mutex);
}

void	single_coder(t_character *chara)
{
	long	timestamp;

	usleep(chara->state->parsing->time_to_burnout * 1000);
	timestamp = get_timestamp_ms(chara);
	pthread_mutex_lock(&chara->state->mutex);
	if (chara->state->simu_state == true)
	{
		printf("%ld %d burned out\n", timestamp, chara->coder->id);
		chara->state->simu_state = false;
	}
	pthread_mutex_unlock(&chara->state->mutex);
	mark_coder_done(chara);
}

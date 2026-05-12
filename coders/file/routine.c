/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:56:39 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/12 15:44:20 by mobenais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

void	coder_refactor(t_character *chara)
{
	long	timestamp;

	if (!get_simu_state(chara->state))
		return ;
	timestamp = get_timestamp_ms(chara);
	secure_log(chara, "is refactoring", timestamp);
	check_state_session(chara->coder->debug, chara);
}

void	coder_debug(t_character *chara)
{
	long	timestamp;

	if (!get_simu_state(chara->state))
		return ;
	timestamp = get_timestamp_ms(chara);
	secure_log(chara, "is debugging", timestamp);
	check_state_session(chara->coder->refractor, chara);
}

void	mark_coder_done(t_character *chara)
{
	pthread_mutex_lock(&chara->state->mutex);
	chara->state->coders_done++;
	pthread_mutex_unlock(&chara->state->mutex);
}

void	single_coder(t_character *chara)
{
	usleep(chara->state->parsing->time_to_burnout * 1000);
	pthread_mutex_lock(&chara->state->mutex);
	if (chara->state->simu_state == true)
	{
		chara->state->simu_state = false;
		pthread_mutex_unlock(&chara->state->mutex);
		secure_log(chara, "burned out", get_timestamp_ms(chara));
	}
	else
		pthread_mutex_unlock(&chara->state->mutex);
	mark_coder_done(chara);
}

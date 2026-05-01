/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:56:39 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/01 16:02:40 by mobenais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

static void	coder_refactor(t_character *chara)
{
	long	timestamp;

	if (!get_simu_state(chara->state))
		return ;
	timestamp = get_timestamp_ms(chara);
	secure_log(chara, "is refactoring", timestamp);
	check_state_session(chara->coder->debug, chara);
}

static void	coder_debug(t_character *chara)
{
	long	timestamp;

	if (!get_simu_state(chara->state))
		return ;
	timestamp = get_timestamp_ms(chara);
	secure_log(chara, "is debugging", timestamp);
	check_state_session(chara->coder->refractor, chara);
}

static void	mark_coder_done(t_character *chara)
{
	pthread_mutex_lock(&chara->state->mutex);
	chara->state->coders_done++;
	pthread_mutex_unlock(&chara->state->mutex);
}

void	*routine_coder(void *arg)
{
	t_character	*chara;

	chara = (t_character *)arg;
	while (chara->coder->routine > 0 && get_simu_state(chara->state))
	{
		if (!get_simu_state(chara->state))
			break ;
		coder_compile(chara);
		coder_refactor(chara);
		if (!get_simu_state(chara->state))
			break ;
		coder_debug(chara);
		chara->coder->routine--;
	}
	mark_coder_done(chara);
	return (NULL);
}

void	coder_compile(t_character *chara)
{
	long	timestamp;

	timestamp = get_timestamp_ms(chara);
	if (!get_simu_state(chara->state))
		return ;
	secure_log(chara, "is compiling", timestamp);
	check_simu_and_check_state(chara);
	pthread_mutex_lock(&chara->coder->mutex);
	gettimeofday(&chara->coder->last_time_compile, NULL);
	pthread_mutex_unlock(&chara->coder->mutex);
	if (get_simu_state(chara->state))
	{
		check_state_session(chara->coder->compile, chara);
		dongle_on_table(chara->coder->left);
		dongle_on_table(chara->coder->right);
	}
}

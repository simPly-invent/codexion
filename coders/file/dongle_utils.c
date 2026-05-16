/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:55:28 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/16 19:17:04 by mobenais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

static void	finalize_dongle_acquisition(t_dongle *dongle, t_character *chara)
{
	if (!get_simu_state(chara->state))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return ;
	}
	heap_pop(dongle->heap, &dongle->heap_len);
	dongle->plugged = true;
	pthread_mutex_unlock(&dongle->mutex);
}

static void	call_fifo(t_dongle *dongle, t_character *chara)
{
	dongle->fifo_counter++;
	heap_push(dongle->heap, &dongle->heap_len, chara->coder,
		dongle->fifo_counter);
	wait_for_dongle_availability(dongle, chara);
	finalize_dongle_acquisition(dongle, chara);
}

static void	call_edf(t_dongle *dongle, t_character *chara)
{
	long	key;

	key = compute_edf_key(chara->coder);
	heap_push(dongle->heap, &dongle->heap_len, chara->coder, key);
	wait_for_dongle_availability(dongle, chara);
	finalize_dongle_acquisition(dongle, chara);
}

void	dongle_in_hand(t_dongle *dongle, t_character *chara)
{
	if (pthread_mutex_lock(&dongle->mutex) == 0)
	{
		if (strcmp(chara->coder->scheduler, "fifo") == 0)
			call_fifo(dongle, chara);
		else if (strcmp(chara->coder->scheduler, "edf") == 0)
			call_edf(dongle, chara);
	}
	secure_log(chara, "has taken a dongle");
}

void	dongle_on_table(t_dongle *dongle)
{
	if (pthread_mutex_lock(&dongle->mutex) == 0)
	{
		dongle->plugged = false;
		pthread_cond_broadcast(&dongle->cond);
		gettimeofday(&dongle->last_time_register, NULL);
		pthread_mutex_unlock(&dongle->mutex);
	}
}

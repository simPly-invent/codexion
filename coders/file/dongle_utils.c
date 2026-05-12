/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:55:28 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/12 15:44:05 by mobenais         ###   ########.fr       */
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
	dongle->plugged = true;
	dongle->len_queu--;
	pthread_mutex_unlock(&dongle->mutex);
}

static void	call_fifo(t_dongle *dongle, t_character *chara)
{
	dongle->cooldown_priority[dongle->len_queu] = chara->coder;
	dongle->len_queu++;
	wait_for_dongle_availability(dongle, chara);
	remove_coder_from_queue(dongle);
	finalize_dongle_acquisition(dongle, chara);
}

static void	call_edf(t_dongle *dongle, t_character *chara)
{
	edf_order(dongle, chara);
	wait_for_dongle_availability(dongle, chara);
	remove_coder_from_queue(dongle);
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
	if (get_simu_state(chara->state))
		secure_log(chara, "has taken a dongle", get_timestamp_ms(chara));
}

void	dongle_on_table(t_dongle *dongle)
{
	if (pthread_mutex_lock(&dongle->mutex) == 0)
	{
		dongle->plugged = false;
		pthread_cond_broadcast(&dongle->cond);
		pthread_mutex_unlock(&dongle->mutex);
	}
}

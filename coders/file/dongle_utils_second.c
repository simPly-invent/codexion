/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils_second.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:55:08 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/11 21:45:02 by mobenais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

void	wait_for_dongle_availability(t_dongle *dongle, t_character *chara)
{
	struct timespec	res;

	while ((dongle->plugged || dongle->cooldown_priority[0] != chara->coder
			|| cooldown_limit(dongle)) && get_simu_state(chara->state))
	{
		res = convert_time_stamp_dongle(dongle);
		pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &res);
	}
}

void	remove_coder_from_queue(t_dongle *dongle)
{
	int	i;

	i = 0;
	while (i < dongle->len_queu - 1)
	{
		dongle->cooldown_priority[i] = dongle->cooldown_priority[i + 1];
		i++;
	}
}

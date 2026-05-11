/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_monitor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:57:10 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/11 21:45:31 by mobenais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>

void	*thread_monitor(void *arg)
{
	t_thmonitor	var;

	var.info = (t_info_monitor *)arg;
	var.res = 0;
	var.k = 0;
	while (get_simu_state(var.info->chara->state))
	{
		var.i = 0;
		var.check = check_burnout(var.i, var.k, var.res, var.info);
		if (var.check < 0)
			return (NULL);
		pthread_mutex_lock(&var.info->chara->state->mutex);
		if (var.info->chara->state->coders_done == var.info->size)
		{
			var.info->chara->state->simu_state = false;
			pthread_mutex_unlock(&var.info->chara->state->mutex);
			broadcast_to_all_dongles(var.info, 0);
			return (NULL);
		}
		pthread_mutex_unlock(&var.info->chara->state->mutex);
		usleep(1000);
	}
	return (NULL);
}

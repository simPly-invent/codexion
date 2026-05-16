/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils_second.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:55:08 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/16 17:11:01 by mobenais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

void	wait_for_dongle_availability(t_dongle *dongle, t_character *chara)
{
	struct timespec	res;

	while ((dongle->plugged || dongle->heap_len == 0
			|| dongle->heap[0].coder != chara->coder
			|| cooldown_limit(dongle)) && get_simu_state(chara->state))
	{
		res = convert_time_stamp_dongle(dongle);
		pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &res);
	}
}

void	*routine_coder(void *arg)
{
	t_character	*chara;

	chara = (t_character *)arg;
	if (chara->size == 1)
		return (single_coder(chara), NULL);
	while (chara->coder->routine > 0 && get_simu_state(chara->state))
	{
		if (coder_compile(chara) == 1)
			break ;
		coder_debug(chara);
		if (!get_simu_state(chara->state))
			break ;
		coder_refactor(chara);
		chara->coder->routine--;
	}
	mark_coder_done(chara);
	return (NULL);
}

int	coder_compile(t_character *chara)
{
	if (!get_simu_state(chara->state))
		return (1);
	check_simu_and_check_state(chara);
	if (chara->coder->left == chara->coder->right)
		return (1);
	if (!get_simu_state(chara->state))
		return (1);
	pthread_mutex_lock(&chara->coder->mutex);
	gettimeofday(&chara->coder->last_time_compile, NULL);
	pthread_mutex_unlock(&chara->coder->mutex);
	secure_log(chara, "is compiling");
	check_state_session(chara->coder->compile, chara);
	dongle_on_table(chara->coder->left);
	dongle_on_table(chara->coder->right);
	return (0);
}

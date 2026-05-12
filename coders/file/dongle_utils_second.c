/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils_second.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:55:08 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/12 15:44:54 by mobenais         ###   ########.fr       */
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

void	*routine_coder(void *arg)
{
	t_character	*chara;

	chara = (t_character *)arg;
	if (chara->size == 1)
		return (single_coder(chara), NULL);
	while (chara->coder->routine > 0 && get_simu_state(chara->state))
	{
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
	if (!get_simu_state(chara->state))
		return ;
	pthread_mutex_lock(&chara->coder->mutex);
	gettimeofday(&chara->coder->last_time_compile, NULL);
	pthread_mutex_unlock(&chara->coder->mutex);
	check_simu_and_check_state(chara);
	if (!get_simu_state(chara->state)
		&& chara->coder->left != chara->coder->right)
		return ;
	secure_log(chara, "is compiling", get_timestamp_ms(chara));
	check_state_session(chara->coder->compile, chara);
	dongle_on_table(chara->coder->left);
	dongle_on_table(chara->coder->right);
}

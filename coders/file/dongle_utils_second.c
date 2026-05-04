/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils_second.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:55:08 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/04 21:42:00 by mobenais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"
#include <stddef.h>
#include <limits.h>

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

static int	ft_is_space(char c)
{
	if (((c >= 9) && (c <= 13))
		|| (c == 32))
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	long long int	result;
	int				sign;
	long long int	i;

	i = 0;
	sign = 1;
	result = 0;
	while (ft_is_space(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			sign *= -1;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (sign >= 0 && result > ((LLONG_MAX - (str[i] - '0')) / 10))
			return (-1);
		else if (sign < 0 && result < ((LLONG_MIN + (str[i] - '0')) / 10))
			return (0);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:54:44 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/11 21:44:48 by mobenais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

static void	normalize_lower_case(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
		i++;
	}
}

int	check_fifo_edf(char *str)
{
	normalize_lower_case(str);
	if (strcmp(str, "edf") == 0 || strcmp(str, "fifo") == 0)
		return (0);
	else
		return (1);
}

void	check_state_session(int ms, t_character *chara)
{
	struct timeval	start;
	struct timeval	now;
	long			elapsed;

	(void)chara;
	gettimeofday(&start, NULL);
	while (1)
	{
		gettimeofday(&now, NULL);
		elapsed = (now.tv_sec - start.tv_sec) * 1000
			+ (now.tv_usec - start.tv_usec) / 1000;
		if (elapsed >= ms)
			return ;
		usleep(200);
	}
}

int	is_valid_int(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

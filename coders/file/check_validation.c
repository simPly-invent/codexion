/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:54:44 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/01 16:39:27 by mobenais         ###   ########.fr       */
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
	int	i;

	i = 0;
	while (i < ms && get_simu_state(chara->state))
	{
		usleep(1000);
		i++;
	}
}

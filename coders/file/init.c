/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:55:48 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/12 15:43:52 by mobenais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"
#include <pthread.h>
#include <sys/time.h>

static void	init_all_coders(t_coder *coders, t_pars *parsing, int size)
{
	int	i;
	int	tmp;

	i = 0;
	tmp = size;
	while (tmp > 0)
	{
		init_coder(&coders[i], parsing, i + 1);
		i++;
		tmp--;
	}
}

static int	init_all_dongles(t_dongle *dongles, t_pars *parsing, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (init_dongle(&dongles[i], parsing, size, i) != 0)
		{
			while (i > 0)
			{
				i--;
				free(dongles[i].heap);
			}
			return (-1);
		}
		i++;
	}
	return (0);
}

static void	link_coders_to_dongles(t_coder *coders, t_dongle *dongles, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		coders[i].left = &dongles[i];
		coders[i].right = &dongles[(i + 1) % size];
		i++;
	}
}

int	init_table(t_coder *coders, t_pars *parsing, t_dongle *dongles, int size)
{
	init_all_coders(coders, parsing, size);
	if (init_all_dongles(dongles, parsing, size) != 0)
		return (-1);
	link_coders_to_dongles(coders, dongles, size);
	return (0);
}

void	init_simu(t_simulation *state, t_pars *parsing, int size)
{
	pthread_mutex_init(&state->mutex, NULL);
	state->simu_state = true;
	state->coders_done = 0;
	state->size = size;
	state->parsing = parsing;
	memset(&state->start, 0, sizeof(struct timeval));
}

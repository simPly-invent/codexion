/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:55:08 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/12 15:24:55 by mobenais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void	free_dongles_priority(t_main *var)
{
	var->i = 0;
	while (var->i < var->size)
	{
		free(var->dongles[var->i].cooldown_priority);
		var->i++;
	}
}

static void	free_allocated_pointers(t_main *var)
{
	free(var->coders);
	free(var->chara);
	free(var->simu);
	free(var->info);
	free(var->dongles);
	if (var->parsing->scheduler)
		free(var->parsing->scheduler);
	free(var->parsing);
}

void	free_all(t_main *var)
{
	free_dongles_priority(var);
	free_allocated_pointers(var);
	free(var);
}

static int	run_simulation(t_main *var, int argc, char **argv)
{
	var->verif = parser(argv, argc - 1, var->parsing);
	if (var->verif < 0)
	{
		free_parser(var);
		return (-1);
	}
	var->size = var->parsing->nbr_coder;
	if (make_heap(var) < 0)
	{
		free_all(var);
		return (-1);
	}
	if (init_thread(var) != 0)
	{
		free_all(var);
		return (-1);
	}
	free_all(var);
	return (0);
}

int	main(int argc, char **argv)
{
	t_main	*var;

	var = malloc(sizeof(t_main));
	if (!var)
		return (1);
	if (make_heap_second(var) < 0)
		return (1);
	return (run_simulation(var, argc, argv));
}

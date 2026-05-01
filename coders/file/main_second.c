/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_second.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:55:08 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/01 16:43:50 by mobenais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

int	make_heap_second(t_main *var)
{
	if (allocate_parsing_structures(var) != 0)
	{
		free(var);
		return (-1);
	}
	init_allocated_structures(var);
	return (0);
}

void	create_all_threads(t_main *var)
{
	var->i = 0;
	while (var->i < var->size)
	{
		create_thread(&var->chara[var->i]);
		var->i++;
	}
}

void	close_all_threads(t_main *var)
{
	var->i = 0;
	while (var->i < var->size)
	{
		close_thread(&var->chara[var->i]);
		var->i++;
	}
}

void	init_thread(t_main *var)
{
	init_monitor(var->info, var->parsing, var->chara, var->coders);
	var->response = create_thread_monitor(var->info, &var->monitor);
	create_all_threads(var);
	close_all_threads(var);
	if (!var->response)
		close_thread_monitor(var->monitor);
}

void	free_parser(t_main *var)
{
	if (var->parsing->scheduler)
		free(var->parsing->scheduler);
	free(var->parsing);
	free(var->simu);
	free(var->info);
	free(var);
}

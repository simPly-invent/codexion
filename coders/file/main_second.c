/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_second.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:55:08 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/12 15:44:41 by mobenais         ###   ########.fr       */
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

int	create_all_threads(t_main *var)
{
	var->i = 0;
	while (var->i < var->size)
	{
		if (create_thread(&var->chara[var->i]) != 0)
		{
			return (-1);
		}
		var->i++;
	}
	return (0);
}

int	close_all_threads(t_main *var)
{
	var->i = 0;
	while (var->i < var->size)
	{
		if (close_thread(&var->chara[var->i]) != 0)
			return (-1);
		var->i++;
	}
	return (0);
}

int	init_thread(t_main *var)
{
	int				i;
	struct timeval	start_time;

	init_monitor(var->info, var->parsing, var->chara, var->coders);
	gettimeofday(&start_time, NULL);
	i = 0;
	while (i < var->size)
	{
		var->chara[i].coder->last_time_compile = start_time;
		i++;
	}
	var->response = create_thread_monitor(var->info, &var->monitor);
	if (!var->response)
		return (-1);
	if (create_all_threads(var) != 0)
		broadcast_to_all_dongles(var->info, 0);
	else if (close_all_threads(var) != 0)
		;
	else
	{
		pthread_join(var->monitor, NULL);
		return (0);
	}
	pthread_join(var->monitor, NULL);
	return (-1);
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

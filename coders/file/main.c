/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:55:08 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/12 14:45:34 by mobenais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

int	allocate_main_structures(t_main *main)
{
	main->coders = malloc(sizeof(t_coder) * main->size);
	if (!main->coders)
		return (-1);
	memset(main->coders, 0, sizeof(t_coder) * main->size);
	main->dongles = malloc(sizeof(t_dongle) * main->size);
	if (!main->dongles)
	{
		free(main->coders);
		return (-1);
	}
	memset(main->dongles, 0, sizeof(t_dongle) * main->size);
	main->chara = malloc(sizeof(t_character) * main->size);
	if (!main->chara)
	{
		free(main->coders);
		free(main->dongles);
		return (-1);
	}
	memset(main->chara, 0, sizeof(t_character) * main->size);
	return (0);
}

void	initialize_characters(t_main *main)
{
	main->i = 0;
	while (main->i < main->size)
	{
		init_character(&main->chara[main->i], &main->coders[main->i],
			main->simu, main->dongles);
		main->i++;
	}
}

int	make_heap(t_main *main)
{
	if (allocate_main_structures(main) != 0)
		return (-1);
	init_simu(main->simu, main->parsing, main->size);
	if (init_table(main->coders, main->parsing, main->dongles, main->size) != 0)
		return (-1);
	initialize_characters(main);
	return (0);
}

int	allocate_parsing_structures(t_main *var)
{
	var->parsing = malloc(sizeof(t_pars));
	if (!var->parsing)
		return (-1);
	var->simu = malloc(sizeof(t_simulation));
	if (!var->simu)
	{
		free(var->parsing);
		return (-1);
	}
	var->info = malloc(sizeof(t_info_monitor));
	if (!var->info)
	{
		free(var->parsing);
		free(var->simu);
		return (-1);
	}
	return (0);
}

void	init_allocated_structures(t_main *var)
{
	memset(var->parsing, 0, sizeof(t_pars));
	memset(var->simu, 0, sizeof(t_simulation));
	memset(var->info, 0, sizeof(t_info_monitor));
}

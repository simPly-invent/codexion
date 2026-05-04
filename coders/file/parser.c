/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:56:19 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/04 21:19:53 by mobenais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"
#include <stdlib.h>

static int	allocate_parser_memory(t_pa *var, int size)
{
	var->tab = malloc(sizeof(int) * size);
	if (!var->tab)
		return (-1);
	var->str = ft_strdup(var->str);
	if (!var->str)
	{
		free(var->tab);
		return (-1);
	}
	return (0);
}

static int	fill_parameters_table(t_pa *var, char **argv, int size)
{
	var->i = 1;
	var->j = 0;
	while (var->i < size)
	{
		if (ft_atoi(argv[var->i]) < 0)
		{
			free(var->tab);
			free(var->str);
			return (-1);
		}
		var->tab[var->j] = ft_atoi(argv[var->i]);
		var->i++;
		var->j++;
	}
	return (0);
}

int	parser(char **argv, int size, t_pars *parsing)
{
	t_pa	var;

	if (size != 8)
		return (-1);
	if (check_fifo_edf(argv[8]) == 1)
		return (-1);
	var.str = argv[size];
	if (allocate_parser_memory(&var, size) != 0)
		return (-1);
	if (fill_parameters_table(&var, argv, size) != 0)
		return (-1);
	init_pars(var.tab, parsing, var.str);
	free(var.tab);
	free(var.str);
	return (0);
}

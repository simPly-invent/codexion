/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:56:19 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/09 11:10:10 by mobenais         ###   ########.fr       */
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

static int check_only_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
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
	if (check_only_digit(argv[1]) == 1 || check_only_digit(argv[2]) == 1 || check_only_digit(argv[3]) == 1
		|| check_only_digit(argv[4]) == 1 || check_only_digit(argv[5]) == 1
		|| check_only_digit(argv[6]) == 1 || check_only_digit(argv[7]) == 1 || strcmp(argv[1], "0") == 0 || strcmp(argv[2], "0") == 0 || strcmp(argv[3], "0") == 0
		|| strcmp(argv[4], "0") == 0 || strcmp(argv[5], "0") == 0 || strcmp(argv[6], "0") == 0 || strcmp(argv[7], "0") == 0)
		return (-1);
	if (allocate_parser_memory(&var, size) != 0)
		return (-1);
	if (fill_parameters_table(&var, argv, size) != 0)
		return (-1);
	init_pars(var.tab, parsing, var.str);
	free(var.tab);
	free(var.str);
	return (0);
}

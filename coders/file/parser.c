#include "../header/codexion.h"
#include <stdlib.h>

int	parser(char **argv, int size, t_pars *parsing)
{
	t_pa	var;

	if (size < 8)
		return (-1);
	var.i = 1;
	var.j = 0;
	if (check_fifo_edf(argv[8]) == 1)
		return (-1);
	var.tab = malloc(sizeof(int) * size);
	if (!var.tab)
		return (-1);
	var.str = ft_strdup(argv[size]);
	if(!var.str)
	{
		free(var.tab);
		return (-1);
	}
	while (var.i < size)
	{
		var.tab[var.j] = atoi(argv[var.i]);
		var.i++;
		var.j++;
	}
	init_pars(var.tab, parsing, var.str);
	free(var.tab);
	free(var.str);
	return (0);
}

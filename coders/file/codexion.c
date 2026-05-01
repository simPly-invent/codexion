#include "../header/codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int	allocate_main_structures(t_main *main)
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

static void	initialize_characters(t_main *main)
{
	main->i = 0;
	while (main->i < main->size)
	{
		init_character(&main->chara[main->i], &main->coders[main->i], main->simu, main->dongles);
		main->i++;
	}
}

static int	make_heap(t_main *main)
{
	if (allocate_main_structures(main) != 0)
		return (-1);
	init_simu(main->simu, main->size);
	if (init_table(main->coders, main->parsing, main->dongles, main->size) != 0)
		return (-1);
	initialize_characters(main);
	return (0);
}

static int	allocate_parsing_structures(t_main *var)
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

static void	init_allocated_structures(t_main *var)
{
	memset(var->parsing, 0, sizeof(t_pars));
	memset(var->simu, 0, sizeof(t_simulation));
	memset(var->info, 0, sizeof(t_info_monitor));
}

static int	make_heap_second(t_main *var)
{
	if (allocate_parsing_structures(var) != 0)
	{
		free(var);
		return (-1);
	}
	init_allocated_structures(var);
	return (0);
}

static void	create_all_threads(t_main *var)
{
	var->i = 0;
	while (var->i < var->size)
	{
		create_thread(&var->chara[var->i]);
		var->i++;
	}
}

static void	close_all_threads(t_main *var)
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

int	main(int argc, char **argv)
{
	t_main *var;
    var = malloc(sizeof(t_main));
    if (!var)
		return (1);

	if (make_heap_second(var) < 0)
		return (1);
	var->verif = parser(argv, argc - 1, var->parsing);
	if (var->verif < 0)
	{
		printf("\nAn error occurse\n");
		free_parser(var);
		return (1);
	}
	var->size = var->parsing->nbr_coder;
	if (make_heap(var) < 0)
	{
		printf("\nMemory allocation failed\n");
		free_all(var);
		return (1);
	}
	init_thread(var);
	pthread_join(var->monitor, NULL);
	free_all(var);
	return (0);
}

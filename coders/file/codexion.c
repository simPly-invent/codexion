#include "../header/codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void	make_heap(t_main *main)
{
	main->i = 0;
	main->coders = malloc(sizeof(t_coder) * main->size);
	if (!main->coders)
		return ;
	main->dongles = malloc(sizeof(t_dongle) * main->size);
	if (!main->dongles)
	{
		free(main->coders);
		return ;
	}
	main->chara = malloc(sizeof(t_character) * main->size);
	if (!main->chara)
	{
		free(main->coders);
		free(main->dongles);
		return ;
	}
	init_simu(main->simu, main->size);
	init_table(main->coders, main->parsing, main->dongles, main->size);
	while (main->i < main->size)
	{
		init_character(&main->chara[main->i], &main->coders[main->i], main->simu, main->dongles);
		main->i++;
	}
}

static int	make_heap_second(t_main *var)
{
	var->parsing = malloc(sizeof(t_pars));
	if (!var->parsing)
	{
		free(var);
		return (-1);
	}
	var->simu = malloc(sizeof(t_simulation));
	if (!var->simu)
	{
		free(var->parsing);
		free(var);
		return (-1);
	}
	var->info = malloc(sizeof(t_info_monitor));
	if (!var->info)
	{
		free(var->parsing);
		free(var->simu);
		free(var);
		return (-1);
	}
	memset(var->parsing, 0, sizeof(t_pars));
	memset(var->simu, 0, sizeof(t_simulation));
	memset(var->info, 0, sizeof(t_info_monitor));
	return 0;
}

void	init_thread(t_main *var)
{
	var->i = 0;
	init_monitor(var->info, var->parsing, var->chara, var->coders);
	var->response = create_thread_monitor(var->info, &var->monitor);
	while (var->i < var->size)
	{
		create_thread(&var->chara[var->i]);
		var->i++;
	}
	var->i = 0;
	while (var->i < var->size)
	{
		close_thread(&var->chara[var->i]);
		var->i++;
	}
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

void	free_all(t_main *var)
{
	var->i = 0;
	while (var->i < var->size) {
		free(var->dongles[var->i].cooldown_priority);
		var->i++;
	}
	free(var->coders);
	free(var->chara);
	free(var->simu);
	free(var->info);
	free(var->dongles);
	free(var->parsing->scheduler);
	free(var->parsing);
	free(var);
}

int	main(int argc, char **argv)
{
	t_main *var;
    var = malloc(sizeof(t_main));
    if (!var)
	{
        return (1);
	}
	if(make_heap_second(var) < 0)
		return 1;
	var->verif = parser(argv, argc - 1, var->parsing);
	if (var->verif < 0)
	{

		printf("\nAn error occurse\n");
		free_parser(var);
		return (0);
	}
	var->size = var->parsing->nbr_coder;
	make_heap(var);
	init_thread(var);
	pthread_join(var->monitor, NULL);
	free_all(var);
	return (0);
}

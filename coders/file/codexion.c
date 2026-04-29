#include "../header/codexion.h"


void	*make_heap(t_main *main)
{
	main->i = 0;
	main->coders = malloc(sizeof(t_coder) * main->size);
	if (!main->coders)
		return (NULL);
	main->dongles = malloc(sizeof(t_dongle) * main->size);
	if (!main->dongles)
	{
		free(main->coders);
		return (NULL);
	}
	main->chara = malloc(sizeof(t_character) * main->size);
	if (!main->chara)
	{
		free(main->coders);
		free(main->dongles);
	}
	init_simu(main->simu, main->size);
	init_table(main->coders, main->parsing, main->dongles, main->size);
	while (main->i < main->size)
	{
		init_character(&main->chara[main->i], &main->coders[main->i], main->simu, main->dongles);
		main->i++;
	}
	return NULL;
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


int	main(int argc, char **argv)
{
	t_main *var;

	var->verif = parser(argv, argc - 1, var->parsing);
	if (var->verif < 0)
	{
		printf("\nAn error occurse\n");
		return (0);
	}
	var->size = var->parsing->nbr_coder;
	make_heap(var);
	init_thread(var);
	pthread_join(var->monitor, NULL);
	free(var->coders);
	return (0);
}

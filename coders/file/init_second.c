#include "../header/codexion.h"

void	init_pars(int *tab, t_pars *parsing, char *str)
{
	parsing->nbr_coder = tab[0];
	parsing->time_to_burnout = tab[1];
	parsing->time_to_compile = tab[2];
	parsing->time_to_debug = tab[3];
	parsing->time_to_refactor = tab[4];
	parsing->number_of_routine_required = tab[5];
	parsing->dgle_cooldown = tab[6];
	parsing->scheduler = ft_strdup(str);
}

void	init_coder(t_coder *any, t_pars *parsing, int id)
{
	any->id = id;
	any->burnout = parsing->time_to_burnout;
	any->compile = parsing->time_to_compile;
	any->debug = parsing->time_to_debug;
	any->refractor = parsing->time_to_refactor;
	any->routine = parsing->number_of_routine_required;
	any->scheduler = parsing->scheduler;
	pthread_mutex_init(&any->mutex, NULL);
	gettimeofday(&any->last_time_compile, NULL);
}

int	init_dongle(t_dongle *dongle, t_pars *parsing, int size, int id)
{
	dongle->id = id;
	dongle->plugged = false;
	pthread_mutex_init(&dongle->mutex, NULL);
	pthread_cond_init(&dongle->cond, NULL);
	dongle->cooldown_priority = malloc(sizeof(t_coder *) * size);
	if (!dongle->cooldown_priority)
		return (-1);
	dongle->len_queu = 0;
	dongle->dongle_cooldown = parsing->dgle_cooldown;
	gettimeofday(&dongle->last_time_register, NULL);
	return (0);
}

void	init_monitor(t_info_monitor *monitor, t_pars *parsing,
		t_character *chara, t_coder *coders)
{
	monitor->coders = coders;
	monitor->parsing = parsing;
	monitor->chara = chara;
	monitor->size = chara->size;
	monitor->dongles = chara->dongles;
}

void	init_character(t_character *bunch, t_coder *coders, t_simulation *state,
		t_dongle *dongles)
{
	bunch->coder = coders;
	bunch->state = state;
	bunch->dongles = dongles;
	bunch->size = state->size;
}
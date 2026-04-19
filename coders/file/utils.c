#include "../header/codexion.h"


void init_env(int *tab, t_pars *parsing)
{
    parsing->nbr_coder = tab[0];
    parsing->time_to_burnout = tab[1];
    parsing->time_to_compile = tab[2];
    parsing->time_to_debug = tab[3];
    parsing->time_to_refactor = tab[4];
    parsing->number_of_compiles_required = tab[5];
    parsing->dongle_cooldown = tab[6];
    parsing->scheduler = tab[7];
}

void init_coder(t_coder *any, t_pars *parsing, int id)
{
    any->id = id;
    any->burnout = parsing->time_to_burnout;
    any->compile = parsing->time_to_compile;
    any->debug = parsing->time_to_debug;
    any->refractor = parsing->time_to_refactor;
}


int check_fifo_edf(char *str)
{
    if (strcmp(str, "edf") == 0 || strcmp(str, "fifo") == 0)
        return 0;
    else
        return 1;
}
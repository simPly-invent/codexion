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

void init_coder()
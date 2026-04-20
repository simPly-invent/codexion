#include "../header/codexion.h"


char *ft_strdup(char *str);
void init_env(int *tab, t_pars *parsing, char *str);
void init_coder(t_coder *any, t_pars *parsing, int id);
void init_dongle(t_dongle *dongle, t_pars *parsing, int size);
int check_fifo_edf(char *str);
static void   normalize_lower_case(char *str);


char *init_env(int *tab, t_pars *parsing, char *str)
{
    parsing->nbr_coder = tab[0];
    parsing->time_to_burnout = tab[1];
    parsing->time_to_compile = tab[2];
    parsing->time_to_debug = tab[3];
    parsing->time_to_refactor = tab[4];
    parsing->number_of_compiles_required = tab[5];
    parsing->dgle_cooldown = tab[6];
    parsing->scheduler = ft_strdup(str);
}

void init_coder(t_coder *any, t_pars *parsing, int id)
{
    any->id = id;
    any->burnout = parsing->time_to_burnout;
    any->compile = parsing->time_to_compile;
    any->debug = parsing->time_to_debug;
    any->refractor = parsing->time_to_refactor;
}

void    init_dongle(t_dongle *dongle, t_pars *parsing, int size)
{
    dongle->plugged = false;
    dongle->priority_queu = malloc(sizeof(t_coder *) * size);
    dongle->len_queu = 0;
    dongle->dongle_cooldown = parsing->dgle_cooldown;
}


char *ft_strdup(char *str)
{
    int i;
    int size;
    char *ptr;

    i = 0;
    size = strlen(str);
    ptr = malloc(sizeof(char) * (size + 1));

    while (str[i])
    {
        ptr[i] = str[i];
        i++;
    }
    ptr[i] = 0;
    return ptr;
}

static void   normalize_lower_case(char *str)
{
    int i = 0;
    while(str[i])
    {
        if(str[i] >= 'A' && str[i] <= 'Z')
            str[i] += 32;
        i++;
    }
}

int check_fifo_edf(char *str)
{
    normalize_lower_case(str);
    if (strcmp(str, "edf") == 0 || strcmp(str, "fifo") == 0)
        return 0;
    else
        return 1;
}
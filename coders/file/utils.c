#include "../header/codexion.h"
#include <pthread.h>
#include <stdbool.h>


char *ft_strdup(char *str);
void init_pars(int *tab, t_pars *parsing, char *str);
void init_coder(t_coder *any, t_pars *parsing, int id);
void init_dongle(t_dongle *dongle, t_pars *parsing, int size, int id);
int check_fifo_edf(char *str);
static void   normalize_lower_case(char *str);


void    init_pars(int *tab, t_pars *parsing, char *str)
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

void init_coder(t_coder *any, t_pars *parsing, int id)
{
    any->id = id;
    any->burnout = parsing->time_to_burnout;
    any->compile = parsing->time_to_compile;
    any->debug = parsing->time_to_debug;
    any->refractor = parsing->time_to_refactor;
    any->routine = parsing->number_of_routine_required;
    gettimeofday(&any->last_time_compile, NULL);
}

void    init_dongle(t_dongle *dongle, t_pars *parsing, int size, int id)
{
    dongle->id = id;
    dongle->plugged = false;
    pthread_mutex_init(&dongle->mutex, NULL);
    dongle->cooldown_priority = malloc(sizeof(t_coder *) * size);
    dongle->len_queu = 0;
    dongle->dongle_cooldown = parsing->dgle_cooldown;
    gettimeofday(&dongle->last_time_register, NULL);
}


void    init_monitor(t_info_monitor *monitor, t_pars *parsing, t_character *chara, t_coder *coders, int size)
{
    monitor->coders = coders;
    monitor->parsing = parsing;
    monitor->chara = chara;
    monitor->size = size;
    monitor->dongles = chara->dongles;
}
void    init_character(t_character *bunch, t_coder *coders, t_simulation *state, t_dongle *dongles)
{
    bunch->coder = coders;
    bunch->state = state;
    bunch->dongles = dongles;
}

void    init_simu(t_simulation *state)
{
    pthread_mutex_init(&state->mutex, NULL);
    state->simu_state = true;
    state->coders_done = 0;
}

long    convert_time_stamp_coder(t_coder *coder)
{
    long result;
    struct timeval res;
    
    gettimeofday(&res, NULL);
    result = (res.tv_sec - coder->last_time_compile.tv_sec) * 1000 + (res.tv_usec - coder->last_time_compile.tv_usec) / 1000;
    return result;
}

struct timespec convert_time_stamp_dongle(t_dongle *dongle)
{
    struct timespec result;
    struct timeval res;
    
    gettimeofday(&res, NULL);
    result.tv_sec = res.tv_sec + (dongle->dongle_cooldown / 1000);
    result.tv_nsec = (res.tv_usec + (dongle->dongle_cooldown % 1000) * 1000) * 1000;
    if (result.tv_nsec >= 1000000000)
    {
        result.tv_sec++;
        result.tv_nsec -= 1000000000;
    }
    return result;
}

bool    cooldown_limit(t_dongle *dongle)
{
    long result;
    struct timeval res;
    
    gettimeofday(&res, NULL);
    result = (res.tv_sec - dongle->last_time_register.tv_sec) * 1000 + (res.tv_usec - dongle->last_time_register.tv_usec) / 1000;
    if (result > dongle->dongle_cooldown)
        return false;
    return true;
}


void    init_table(t_coder *coders, t_pars *parsing, t_dongle *dongles, int size)
{
    int i;
    int tmp;

    i = 0;
    tmp = size;
    while(tmp > 0)
    {
        init_coder(&coders[i], parsing, i + 1);
        i++;
        tmp--;
    }
    i = 0;
    while(i < size)
    {
        init_dongle(&dongles[i], parsing, size, i);
        i++;
    }
    i = 0;
    while(i < size)
    {
        coders[i].left = &dongles[i];
        coders[i].right = &dongles[(i + 1) % size];
        i++;
    }
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

void    loading_screen()
{
    int i;

    i = 0;
    printf("\033c");
    while(i < 3)
    {
        printf(".");
        fflush(stdout);
        sleep(1);
        i++;
    }
}
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>


// typedef struct s_dongle
// {

// }   t_dongle;

// typedef struct s_coder
// {

// }   t_coder;


typedef struct s_pars
{
    int nbr_coder;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;
    int scheduler;
}   t_pars;


typedef struct s_coder
{
    pthread_t   thread;
    int         id;
    int         routine;
    int         compile;
    int         debug;
    int         refractor;
}               t_coder;



int     create_thread(t_coder anyone);
int     close_thread(t_coder anyone);
void    *routine_coder(void *arg);
int     *parser(char **argv, int size, t_pars parsing);
int     init_env(int *tab, t_pars parsing);
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct s_dongle t_dongle;
typedef struct s_coder t_coder;


typedef struct s_pars
{
    int nbr_coder;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
    int dgle_cooldown;
    char *scheduler;
}   t_pars;



typedef struct s_dongle
{
    bool            plugged;
    pthread_mutex_t mutex;
    t_coder         **priority_queu;
    int             len_queu;
    int             dongle_cooldown;
    struct timeval  last_time_register;
}   t_dongle;


typedef struct s_coder
{
    pthread_t   thread;
    int         id;
    int         burnout;
    int         compile;
    int         debug;
    int         refractor;
    t_dongle    *left;
    t_dongle    *right;
}               t_coder;

void    init_coder(t_coder *any, t_pars *parsing, int id);
void    init_dongle(t_dongle *dongle, t_pars *parsing, int size);
char    *init_env(int *tab, t_pars *parsing, char *str);
char    *ft_strdup(char *str);
void    *routine_coder(void *arg);
int     create_thread(t_coder *anyone);
int     close_thread(t_coder *anyone);
int     parser(char **argv, int size, t_pars *parsing);
int     check_fifo_edf(char *str);
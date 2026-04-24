#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct s_dongle t_dongle;
typedef struct s_coder t_coder;
typedef struct s_simulation t_simulation;

typedef struct s_pars
{
    int nbr_coder;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_routine_required;
    int dgle_cooldown;
    char *scheduler;
}   t_pars;


typedef struct s_dongle
{
    bool            plugged;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    t_coder         **cooldown_priority;
    int             len_queu;
    int             dongle_cooldown;
    struct timeval  last_time_register;
}   t_dongle;


typedef struct s_coder
{
    pthread_t       thread;
    int             id;
    int             routine;
    int             burnout;
    int             compile;
    int             debug;
    int             refractor;
    t_dongle        *left;
    t_dongle        *right;
    struct timeval  last_time_compile;
}               t_coder;

typedef struct s_simulation
{
    pthread_mutex_t  mutex; 
    bool        simu_state;
}           t_simulation;

typedef struct s_info_monitor
{
    t_pars       *parsing;
    t_simulation *state;
    t_coder      *coders;
}       t_info_monitor;


void    init_coder(t_coder *any, t_pars *parsing, int id);
void    init_dongle(t_dongle *dongle, t_pars *parsing, int size);
void    init_table(t_coder *coders, t_pars *parsing, t_dongle *dongles, int size);
void    init_monitor(t_info_monitor *monitor, t_pars *parsing, t_simulation *state, t_coder *coders);
void    init_pars(int *tab, t_pars *parsing, char *str);
void    dongle_in_hand(t_dongle *dongle);
void    *routine_coder(void *arg);
void    coder_compile(t_coder *coder);
void    thread_monitor(t_info_monitor *info, int size);
int     create_thread(t_coder *anyone);
int     close_thread(t_coder *anyone);
int     parser(char **argv, int size, t_pars *parsing);
int     check_fifo_edf(char *str);
char    *ft_strdup(char *str);
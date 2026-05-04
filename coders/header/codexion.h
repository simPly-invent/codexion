/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:55:08 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/04 21:42:11 by mobenais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <bits/types/struct_timeval.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <sys/time.h>

typedef struct s_pars			t_pars;
typedef struct s_simulation		t_simulation;
typedef struct s_dongle			t_dongle;
typedef struct s_coder			t_coder;
typedef struct s_character		t_character;
typedef struct s_info_monitor	t_info_monitor;

typedef struct s_main
{
	int				i;
	int				size;
	int				verif;
	bool			response;
	t_pars			*parsing;
	t_coder			*coders;
	t_dongle		*dongles;
	t_info_monitor	*info;
	t_simulation	*simu;
	pthread_t		monitor;
	t_character		*chara;
}	t_main;

typedef struct s_pars
{
	int		nbr_coder;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_routine_required;
	int		dgle_cooldown;
	char	*scheduler;
}	t_pars;

typedef struct s_dongle
{
	int				id;
	bool			plugged;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	t_coder			**cooldown_priority;
	int				len_queu;
	int				dongle_cooldown;
	struct timeval	last_time_register;
}	t_dongle;

typedef struct s_coder
{
	pthread_t		thread;
	int				id;
	int				routine;
	int				burnout;
	int				compile;
	int				debug;
	int				refractor;
	t_dongle		*left;
	t_dongle		*right;
	char			*scheduler;
	pthread_mutex_t	mutex;
	struct timeval	last_time_compile;
}	t_coder;

typedef struct s_simulation
{
	pthread_mutex_t	mutex;
	bool			simu_state;
	int				coders_done;
	int				size;
	struct timeval	start;
}	t_simulation;

typedef struct s_info_monitor
{
	t_pars			*parsing;
	t_character		*chara;
	t_coder			*coders;
	t_dongle		*dongles;
	int				size;
}	t_info_monitor;

typedef struct s_character
{
	t_coder			*coder;
	t_simulation	*state;
	t_dongle		*dongles;
	int				size;
}	t_character;

typedef struct s_pa
{
	int		i;
	int		j;
	int		*tab;
	char	*str;
}	t_pa;

typedef struct s_po
{
	int				i;
	struct timespec	res;
}	t_po;

typedef struct s_pb
{
	int		i;
	int		j;
	long	comp;
	long	tmp;
}	t_pb;

typedef struct s_thmonitor
{
	int				i;
	int				k;
	int				check;
	long			res;
	t_info_monitor	*info;
}	t_thmonitor;

void			init_coder(t_coder *any, t_pars *parsing, int id);
int				init_dongle(t_dongle *dongle, t_pars *parsing, int size,
					int id);
int				init_table(t_coder *coders, t_pars *parsing, t_dongle *dongles,
					int size);
void			init_monitor(t_info_monitor *monitor, t_pars *parsing,
					t_character *chara, t_coder *coders);
void			init_pars(int *tab, t_pars *parsing, char *str);
void			init_simu(t_simulation *state, int size);
void			dongle_in_hand(t_dongle *dongle, t_character *chara, long time);
void			dongle_on_table(t_dongle *dongle);
int				check_burnout(int i, int k, long res, t_info_monitor *info);
void			*routine_coder(void *arg);
void			*thread_monitor(void *info);
void			check_state_session(int ms, t_character *chara);
void			coder_compile(t_character *chara);
long			convert_time_stamp_coder(t_coder *coder);
struct timespec	convert_time_stamp_dongle(t_dongle *dongle);
void			init_character(t_character *bunch, t_coder *coders,
					t_simulation *state, t_dongle *dongles);
int				create_thread(t_character *anyone);
int				close_thread(t_character *anyone);
bool			create_thread_monitor(t_info_monitor *monitor,
					pthread_t *thread_);
bool			close_thread_monitor(pthread_t thread_);
bool			cooldown_limit(t_dongle *dongle);
int				parser(char **argv, int size, t_pars *parsing);
int				check_fifo_edf(char *str);
char			*ft_strdup(char *str);
void			check_simu_and_check_state(t_character *chara);
void			edf_order(t_dongle *dongle, t_character *chara);
bool			get_simu_state(t_simulation *state);
long			get_timestamp_ms(t_character *chara);
void			secure_log(t_character *chara, char *message, long timestamp);
void			secure_message(t_character *chara, char *message);
void			get_dongles_ordered(t_character *chara, t_dongle **first,
					t_dongle **second);
void			broadcast_to_all_dongles(t_info_monitor *info, int k);
void			stop_simulation_burnout(t_info_monitor *info, long res, int i,
					int k);
void			init_allocated_structures(t_main *var);
int				ft_atoi(const char *str);
int				allocate_parsing_structures(t_main *var);
int				make_heap(t_main *main);
void			initialize_characters(t_main *main);
int				allocate_main_structures(t_main *main);
int				make_heap_second(t_main *var);
void			create_all_threads(t_main *var);
void			close_all_threads(t_main *var);
void			init_thread(t_main *var);
void			free_parser(t_main *var);
void			wait_for_dongle_availability(t_dongle *dongle,
					t_character *chara);
void			remove_coder_from_queue(t_dongle *dongle);

#endif
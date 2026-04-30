#include "../header/codexion.h"
#include <pthread.h>
#include <sys/time.h>

void	init_table(t_coder *coders, t_pars *parsing, t_dongle *dongles,
		int size)
{
	int	i;
	int	tmp;

	i = 0;
	tmp = size;
	while (tmp > 0)
	{
		init_coder(&coders[i], parsing, i + 1);
		i++;
		tmp--;
	}
	i = 0;
	while (i < size)
	{
		init_dongle(&dongles[i], parsing, size, i);
		i++;
	}
	i = 0;
	while (i < size)
	{
		coders[i].left = &dongles[i];
		coders[i].right = &dongles[(i + 1) % size];
		i++;
	}
}

void	init_simu(t_simulation *state, int size)
{
	pthread_mutex_init(&state->mutex, NULL);
	state->simu_state = true;
	state->coders_done = 0;
	state->size = size;
	gettimeofday(&state->start, NULL);
}

long	convert_time_stamp_coder(t_coder *coder)
{
	long			result;
	struct timeval	res;
	pthread_mutex_lock(&coder->mutex);
	gettimeofday(&res, NULL);
	result = (res.tv_sec - coder->last_time_compile.tv_sec) * 1000
		+ (res.tv_usec - coder->last_time_compile.tv_usec) / 1000;
	pthread_mutex_unlock(&coder->mutex);
	return (result);
}


long	get_timestamp_ms(t_character *chara)
{
	long			result;
	struct timeval	res;

	gettimeofday(&res, NULL);
	result = (res.tv_sec - chara->state->start.tv_sec) * 1000 + (res.tv_usec - chara->state->start.tv_usec) / 1000;
	return (result);
}
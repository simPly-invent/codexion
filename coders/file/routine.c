#include "../header/codexion.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>	


void	*routine_coder(void *arg)
{
	long 		timestamp;
	t_character	*chara;

	chara = (t_character *)arg;
	while (chara->coder->routine > 0 && get_simu_state(chara->state))
	{
		if (!get_simu_state(chara->state))
			break ;
		timestamp = get_timestamp_ms(chara);
		coder_compile(chara);
		secure_log(chara, "is compiling", timestamp);
		if (!get_simu_state(chara->state))
			break ;
		timestamp = get_timestamp_ms(chara);
		secure_log(chara, "is refactoring", timestamp);
		check_state_session(chara->coder->debug, chara);
		if (!get_simu_state(chara->state))
			break ;
		timestamp = get_timestamp_ms(chara);
		secure_log(chara, "is debugging", timestamp);
		check_state_session(chara->coder->refractor, chara);
		chara->coder->routine--;
	}
	pthread_mutex_lock(&chara->state->mutex);
	chara->state->coders_done++;
	pthread_mutex_unlock(&chara->state->mutex);
	return (NULL);
}

void	coder_compile(t_character *chara)
{
	check_simu_and_check_state(chara);
	pthread_mutex_lock(&chara->coder->mutex);
	gettimeofday(&chara->coder->last_time_compile, NULL);
	pthread_mutex_unlock(&chara->coder->mutex);
	if (get_simu_state(chara->state))
	{
		check_state_session(chara->coder->compile, chara);
		dongle_on_table(chara->coder->left);
		dongle_on_table(chara->coder->right);
	}
}

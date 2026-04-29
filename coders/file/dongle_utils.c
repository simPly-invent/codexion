#include "../header/codexion.h"


static void	call_fifo(t_dongle *dongle, t_character *chara)
{	
	int i;
	struct timespec	res;

	i = 0;
	dongle->cooldown_priority[dongle->len_queu] = chara->coder;
	dongle->len_queu++;
	while(dongle->plugged || dongle->cooldown_priority[0] != chara->coder || cooldown_limit(dongle))
	{
		res = convert_time_stamp_dongle(dongle);
		pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &res);
	}
	while (i < dongle->len_queu - 1)
	{
		dongle->cooldown_priority[i] = dongle->cooldown_priority[i + 1];
		i++;
	}
	if (!chara->state->simu_state)
	{
		pthread_mutex_unlock(&dongle->mutex);
		return ;
	}
	else
		dongle->plugged = true;
	dongle->len_queu--;
	pthread_mutex_unlock(&dongle->mutex);
}


static void	call_edf(t_dongle *dongle, t_character *chara)
{
	int i;
	struct timespec	res;

	i = 0;

	edf_order(dongle, chara);
	while ((dongle->plugged || cooldown_limit(dongle) || (dongle->cooldown_priority[0] != chara->coder)) && (chara->state->simu_state))
	{
		res = convert_time_stamp_dongle(dongle);
		pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &res);
	}
	while (i < dongle->len_queu - 1)
	{
		dongle->cooldown_priority[i] = dongle->cooldown_priority[i + 1];
		i++;
	}
	if (!chara->state->simu_state)
	{
		pthread_mutex_unlock(&dongle->mutex);
		return ;
	}
	else
		dongle->plugged = true;
	dongle->len_queu--;
	pthread_mutex_unlock(&dongle->mutex);
}

void	dongle_in_hand(t_dongle *dongle, t_character *chara, long time)
{
	if (pthread_mutex_lock(&dongle->mutex) == 0)
	{
		if (strcmp(chara->coder->scheduler, "fifo") == 0)
			call_fifo(dongle, chara);
		else if(strcmp(chara->coder->scheduler, "edf") == 0)
			call_edf(dongle, chara);
	}
	printf("\ntime: %ld, Coder id.%d is taken dongle\n", time,  chara->coder->id);
}

void	dongle_on_table(t_dongle *dongle)
{
	if (pthread_mutex_lock(&dongle->mutex) == 0)
	{
		dongle->plugged = false;
		pthread_cond_broadcast(&dongle->cond);
		pthread_mutex_unlock(&dongle->mutex);
	}
}
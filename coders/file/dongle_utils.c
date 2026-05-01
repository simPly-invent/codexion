#include "../header/codexion.h"


static void	wait_for_dongle_availability(t_dongle *dongle, t_character *chara, bool is_fifo)
{
	struct timespec res;

	if (is_fifo)
	{
		while ((dongle->plugged || dongle->cooldown_priority[0] != chara->coder || cooldown_limit(dongle)) && get_simu_state(chara->state))
		{
			res = convert_time_stamp_dongle(dongle);
			pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &res);
		}
	}
	else
	{
		while ((dongle->plugged || cooldown_limit(dongle) || (dongle->cooldown_priority[0] != chara->coder)) && (get_simu_state(chara->state)))
		{
			res = convert_time_stamp_dongle(dongle);
			pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &res);
		}
	}
}

static void	remove_coder_from_queue(t_dongle *dongle)
{
	int i;

	i = 0;
	while (i < dongle->len_queu - 1)
	{
		dongle->cooldown_priority[i] = dongle->cooldown_priority[i + 1];
		i++;
	}
}

static void	finalize_dongle_acquisition(t_dongle *dongle, t_character *chara)
{
	if (!get_simu_state(chara->state))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return ;
	}
	dongle->plugged = true;
	dongle->len_queu--;
	pthread_mutex_unlock(&dongle->mutex);
}

static void	call_fifo(t_dongle *dongle, t_character *chara)
{
	dongle->cooldown_priority[dongle->len_queu] = chara->coder;
	dongle->len_queu++;
	wait_for_dongle_availability(dongle, chara, true);
	remove_coder_from_queue(dongle);
	finalize_dongle_acquisition(dongle, chara);
}

static void	call_edf(t_dongle *dongle, t_character *chara)
{
	edf_order(dongle, chara);
	wait_for_dongle_availability(dongle, chara, false);
	remove_coder_from_queue(dongle);
	finalize_dongle_acquisition(dongle, chara);
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
	if (get_simu_state(chara->state))
		printf("%ld %d has taken a dongle\n", time,  chara->coder->id);
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
#include "../header/codexion.h"

void	dongle_in_hand(t_dongle *dongle, t_character *chara, long time)
{
	struct timespec	res;


	if (pthread_mutex_lock(&dongle->mutex) == 0)
	{
		while ((dongle->plugged || cooldown_limit(dongle)) && chara->state->simu_state)
		{
			res = convert_time_stamp_dongle(dongle);
			pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &res);
		}
		if (!chara->state->simu_state)
		{
			pthread_mutex_unlock(&dongle->mutex);
			return ;
		}
		else
			dongle->plugged = true;
		pthread_mutex_unlock(&dongle->mutex);
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
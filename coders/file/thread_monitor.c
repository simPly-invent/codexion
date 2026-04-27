#include "../header/codexion.h"

void	*thread_monitor(void *arg)
{
	t_thmonitor	var;

	var.info = (t_info_monitor *)arg;
	var.res = 0;
	var.k = 0;
	while (var.info->chara->state->simu_state)
	{
		var.i = 0;
		var.check = check_burnout(var.i, var.k, var.res, var.info);
		if (var.check < 0)
			return (NULL);
		if (var.info->chara->state->coders_done == var.info->size)
		{
			var.info->chara->state->simu_state = false;
			while (var.k < var.info->size)
			{
				pthread_cond_broadcast(&var.info->dongles[var.k].cond);
				var.k++;
			}
		}
		usleep(1000);
	}
	return (NULL);
}
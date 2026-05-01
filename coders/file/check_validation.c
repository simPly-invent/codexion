#include "../header/codexion.h"

static void	get_dongles_ordered(t_character *chara, t_dongle **first, t_dongle **second)
{
	if (chara->coder->left->id > chara->coder->right->id)
	{
		*first = chara->coder->right;
		*second = chara->coder->left;
	}
	else
	{
		*first = chara->coder->left;
		*second = chara->coder->right;
	}
}

void	check_simu_and_check_state(t_character *chara)
{
	long		res;
	t_dongle	*first;
	t_dongle	*second;

	res = convert_time_stamp_coder(chara->coder);
	get_dongles_ordered(chara, &first, &second);
	
	if (get_simu_state(chara->state))
		dongle_in_hand(first, chara, res);
	else
		return ;
	if (get_simu_state(chara->state))
		dongle_in_hand(second, chara, res);
}

static void	broadcast_to_all_dongles(t_info_monitor *info, int k)
{
	while (k < info->size)
	{
		pthread_cond_broadcast(&info->dongles[k].cond);
		k++;
	}
}

static void	stop_simulation_burnout(t_info_monitor *info, long res, int i, int k)
{
	if (pthread_mutex_lock(&info->chara->state->mutex) == 0)
	{
		printf("%ld %d burned out\n", res, info->coders[i].id);
		info->chara->state->simu_state = false;
		broadcast_to_all_dongles(info, k);
		pthread_mutex_unlock(&info->chara->state->mutex);
	}
}

int	check_burnout(int i, int k, long res, t_info_monitor *info)
{
	while (i < info->size)
	{
		res = convert_time_stamp_coder(&info->chara->coder[i]);
		if (res > info->parsing->time_to_burnout)
		{
			stop_simulation_burnout(info, res, i, k);
			return (-1);
		}
		i++;
	}
	return (0);
}


static void	normalize_lower_case(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
		i++;
	}
}

int	check_fifo_edf(char *str)
{
	normalize_lower_case(str);
	if (strcmp(str, "edf") == 0 || strcmp(str, "fifo") == 0)
		return (0);
	else
		return (1);
}

void	check_state_session(int ms, t_character *chara)
{
	int	i;

	i = 0;
	while (i < ms && get_simu_state(chara->state))
	{
		usleep(1000);
		i++;
	}
}
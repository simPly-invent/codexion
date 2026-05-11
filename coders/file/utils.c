/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:57:29 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/01 16:18:19 by mobenais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"
#include <string.h>

struct timespec	convert_time_stamp_dongle(t_dongle *dongle)
{
	struct timespec	result;
	struct timeval	res;

	gettimeofday(&res, NULL);
	result.tv_sec = res.tv_sec + (dongle->dongle_cooldown / 1000);
	result.tv_nsec = (res.tv_usec + (dongle->dongle_cooldown % 1000) * 1000)
		* 1000;
	if (result.tv_nsec >= 1000000000)
	{
		result.tv_sec++;
		result.tv_nsec -= 1000000000;
	}
	return (result);
}

bool	cooldown_limit(t_dongle *dongle)
{
	long			result;
	struct timeval	res;

	gettimeofday(&res, NULL);
	result = (res.tv_sec - dongle->last_time_register.tv_sec) * 1000
		+ (res.tv_usec - dongle->last_time_register.tv_usec) / 1000;
	if (result > dongle->dongle_cooldown)
		return (false);
	return (true);
}

char	*ft_strdup(char *str)
{
	int		i;
	int		size;
	char	*ptr;

	if (!str)
		return (NULL);
	i = 0;
	size = strlen(str);
	ptr = malloc(sizeof(char) * (size + 1));
	if (!ptr)
		return (NULL);
	while (str[i])
	{
		ptr[i] = str[i];
		if (ptr[i] >= 'A' && ptr[i] <= 'Z')
			ptr[i] += 32;
		i++;
	}
	ptr[i] = 0;
	return (ptr);
}

static int	check_burnout_edf(t_pb *var, t_dongle *dongle)
{
	while (var->i < dongle->len_queu)
	{
		pthread_mutex_lock(&dongle->cooldown_priority[var->i]->mutex);
		var->tmp = dongle->cooldown_priority[var->i]->last_time_compile.tv_sec
			* 1000
			+ dongle->cooldown_priority[var->i]->last_time_compile.tv_usec
			/ 1000 + dongle->cooldown_priority[var->i]->burnout;
		pthread_mutex_unlock(&dongle->cooldown_priority[var->i]->mutex);
		if (var->tmp > var->comp)
		{
			var->j = var->i;
			break ;
		}
		var->i++;
	}
	return (var->j);
}

void	edf_order(t_dongle *dongle, t_character *chara)
{
	t_pb	var;

	var.i = 0;
	var.j = dongle->len_queu;
	var.comp = 0;
	var.tmp = 0;
	pthread_mutex_lock(&chara->coder->mutex);
	var.comp = chara->coder->last_time_compile.tv_sec * 1000
		+ chara->coder->last_time_compile.tv_usec / 1000
		+ chara->coder->burnout;
	pthread_mutex_unlock(&chara->coder->mutex);
	var.j = check_burnout_edf(&var, dongle);
	var.i = dongle->len_queu;
	while (var.i > var.j)
	{
		dongle->cooldown_priority[var.i] = dongle->cooldown_priority[var.i - 1];
		var.i--;
	}
	dongle->cooldown_priority[var.j] = chara->coder;
	dongle->len_queu++;
}

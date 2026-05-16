/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 15:57:29 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/12 14:46:18 by mobenais         ###   ########.fr       */
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

long	compute_edf_key(t_coder *coder)
{
	long	deadline;

	pthread_mutex_lock(&coder->mutex);
	deadline = coder->last_time_compile.tv_sec * 1000
		+ coder->last_time_compile.tv_usec / 1000
		+ coder->burnout;
	pthread_mutex_unlock(&coder->mutex);
	return (deadline);
}

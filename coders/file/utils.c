#include "../header/codexion.h"

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

	i = 0;
	size = strlen(str);
	ptr = malloc(sizeof(char) * (size + 1));
	while (str[i])
	{
		ptr[i] = str[i];
		i++;
	}
	ptr[i] = 0;
	return (ptr);
}

void	loading_screen(void)
{
	int	i;

	i = 0;
	printf("\033c");
	while (i < 3)
	{
		printf(".");
		fflush(stdout);
		sleep(1);
		i++;
	}
}
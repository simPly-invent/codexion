#include "../header/codexion.h"


int	create_thread(t_character *anyone)
{
	if (pthread_create(&anyone->coder->thread, NULL, &routine_coder,
			anyone) != 0)
		return (1);
	return (0);
}

int	close_thread(t_character *anyone)
{
	if (pthread_join(anyone->coder->thread, NULL) != 0)
		return (1);
	return (0);
}

bool	create_thread_monitor(t_info_monitor *monitor, pthread_t *thread_)
{
	if (pthread_create(thread_, NULL, &thread_monitor, monitor) != 0)
		return (false);
	return (true);
}

bool	close_thread_monitor(pthread_t thread_)
{
	if (pthread_join(thread_, NULL) != 0)
		return (false);
	return (true);
}

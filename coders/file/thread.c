#include "../header/codexion.h"

int     create_thread(t_coder *anyone)
{
    if (pthread_create(&anyone->thread, NULL, &routine_coder, anyone) != 0)
        return 1;
    return 0;
}


int     close_thread(t_coder *anyone)
{
    if (pthread_join(anyone->thread, NULL) != 0)
        return 1;
    return 0;
}

void     take_dongle(t_coder *coder, t_dongle *dongle)
{
    if (pthread_mutex_lock(&dongle->mutex) == 0)
    {
        if (!dongle->plugged)
            dongle->plugged = true;
        pthread_mutex_unlock(&dongle->mutex);
    }
    else
        pthread_cond_wait(&dongle->cond, &dongle->mutex);
}

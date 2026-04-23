#include "../header/codexion.h"
#include <pthread.h>
#include <stdbool.h>

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

void     dongle_in_hand(t_dongle *dongle)
{
    if (pthread_mutex_lock(&dongle->mutex) == 0)
    {
        while (dongle->plugged)
            pthread_cond_wait(&dongle->cond, &dongle->mutex);
        dongle->plugged = true;
        pthread_mutex_unlock(&dongle->mutex);
    }
}


void    dongle_on_table(t_dongle *dongle)
{
    if (pthread_mutex_lock(&dongle->mutex) == 0)
    {
        dongle->plugged = false;
        pthread_cond_broadcast(&dongle->cond);
        pthread_mutex_unlock(&dongle->mutex);
    }
}

void    simu_monitor()
{}
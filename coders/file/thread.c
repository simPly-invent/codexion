#include "../header/codexion.h"
#include <pthread.h>
#include <stdbool.h>

int     create_thread(t_character *anyone)
{
    if (pthread_create(&anyone->coder->thread, NULL, &routine_coder, anyone) != 0)
        return 1;
    return 0;
}


int     close_thread(t_character *anyone)
{
    if (pthread_join(anyone->coder->thread, NULL) != 0)
        return 1;
    return 0;
}

bool     create_thread_monitor(t_info_monitor *monitor, pthread_t *thread_)
{
    if (pthread_create(thread_, NULL, &thread_monitor, monitor) != 0)
        return false;
    return true;
}


bool     close_thread_monitor(pthread_t thread_)
{
    if (pthread_join(thread_, NULL) != 0)
        return false;
    return true;
}


void     dongle_in_hand(t_dongle *dongle, bool *simu)
{
    struct timespec res;
    if (pthread_mutex_lock(&dongle->mutex) == 0)
    {
        while ((dongle->plugged || cooldown_limit(dongle)) && *simu)
        {
            res = convert_time_stamp_dongle(dongle);
            pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &res);
        }
        if (!*simu){
            pthread_mutex_unlock(&dongle->mutex);
            return ;
        }
        else
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

void    *thread_monitor(void *arg)
{
    int     i;
    int     k;
    long    res;
    t_info_monitor *info;

    info = (t_info_monitor *)arg;
    res = 0;
    k = 0;
    while (info->chara->state->simu_state)
    {
        i = 0;
        while (i < info->size)
        {
            res = convert_time_stamp_coder(&info->chara->coder[i]);
            // printf("\nid = %d | res = %ld\n", info->chara->coder[i].id, res);
            if (res > info->parsing->time_to_burnout)
            {
                if (pthread_mutex_lock(&info->chara->state->mutex) == 0)
                {
                    printf("\nThe coder id.%d will burnout\n", info->coders[i].id);
                    info->chara->state->simu_state = false;
                    while (k < info->size)
                    {
                        pthread_cond_broadcast(&info->dongles[k].cond);
                        k++;
                    }
                    pthread_mutex_unlock(&info->chara->state->mutex);
                }
                return (NULL);
            }
            // printf("\nDone = %d\n", info->chara->state->coders_done);
            i++;
        }    
        if(info->chara->state->coders_done == info->size){
            info->chara->state->simu_state = false;
            while (k < info->size)
            {
                pthread_cond_broadcast(&info->dongles[k].cond);
                k++;
            }
        }
        usleep(1000);
    }
    return (NULL);
}

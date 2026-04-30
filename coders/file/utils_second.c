#include "../header/codexion.h"
#include <pthread.h>
#include <stdio.h>


bool    get_simu_state(t_simulation *state)
{
    bool result;
    pthread_mutex_lock(&state->mutex);
    result = state->simu_state;
    pthread_mutex_unlock(&state->mutex);
    return result;
}

void   secure_log(t_character *chara, char *message, long timestamp)
{
    pthread_mutex_lock(&chara->state->mutex);
    printf("%ld %d %s\n", timestamp, chara->coder->id, message);
    pthread_mutex_unlock(&chara->state->mutex);
}


void   secure_message(t_character *chara, char *message)
{
    pthread_mutex_lock(&chara->state->mutex);
    printf("%s", message);
    pthread_mutex_unlock(&chara->state->mutex);
}
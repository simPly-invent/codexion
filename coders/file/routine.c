#include "../header/codexion.h"
#include <stdio.h>
#include <unistd.h>


void    *routine_coder(void *arg)
{   
    t_character *chara;

    chara = (t_character *)arg;
    while (chara->coder->routine > 0 && chara->state->simu_state)
    {
        if (!chara->state->simu_state)
            break;
        coder_compile(chara);
        if (!chara->state->simu_state)
            break;
        printf("\nCoder id.%d is debug\n", chara->coder->id);
        usleep(chara->coder->debug * 1000);
        printf("\nCoder id.%d is refractor\n", chara->coder->id);
        usleep(chara->coder->refractor * 1000);
        chara->coder->routine--;
    }
    chara->state->coders_done++;
    return (NULL);
}


void     coder_compile(t_character *chara)
{
    if (chara->coder->left->id > chara->coder->right->id)
    {   if(chara->state->simu_state)
            dongle_in_hand(chara->coder->right, &chara->state->simu_state);
        else
            return ;
        if(chara->state->simu_state)
            dongle_in_hand(chara->coder->left, &chara->state->simu_state);
        else
            return ;
    }
    else
    {
        if(chara->state->simu_state)
            dongle_in_hand(chara->coder->left, &chara->state->simu_state);
        else
            return ;
        if(chara->state->simu_state)
            dongle_in_hand(chara->coder->right, &chara->state->simu_state);
        else
            return ;
    }
    // printf("oui id.%d", chara->coder->id);
    gettimeofday(&chara->coder->last_time_compile, NULL);
    if(chara->state->simu_state){
        printf("\nCoder id.%d is compile\n", chara->coder->id);
        usleep(chara->coder->compile * 1000);
        dongle_on_table(chara->coder->left);
        dongle_on_table(chara->coder->right);
    }
}
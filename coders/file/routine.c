#include "../header/codexion.h"
#include <stdio.h>
#include <unistd.h>


void    *routine_coder(void *arg)
{   
    t_coder *coder;

    coder = (t_coder *)arg;
    while (coder->routine > 0)
    {
        coder_compile(coder);
        printf("\nCoder id.%d is debug\n", coder->id);
        usleep(coder->debug);
        printf("\nCoder id.%d is refractor\n", coder->id);
        usleep(coder->refractor);
        coder->routine--;
    }
    return (NULL);
}


void     coder_compile(t_coder *coder)
{
    dongle_in_hand(coder->left);
    dongle_in_hand(coder->right);
    printf("\nCoder id.%d is compile\n", coder->id);
    usleep(coder->compile);
}
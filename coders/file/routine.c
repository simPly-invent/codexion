#include "../header/codexion.h"


void    *routine_coder(void *arg)
{   
    t_coder *coder;

    coder = (t_coder *)arg;
    printf("\nCoder id.%d is compile\n", coder->id);
    sleep(coder->compile);
    printf("\nCoder id.%d is debug\n", coder->id);
    sleep(coder->debug);
    printf("\nCoder id.%d is refractor\n", coder->id);
    sleep(coder->refractor);
    return (NULL);
}
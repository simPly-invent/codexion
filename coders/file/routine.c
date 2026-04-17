#include "../header/codexion.h"


void    *routine_coder(void *arg)
{   
    t_coder *coder;

    coder = (t_coder *)arg;
    printf("Coder id.%d is compile", coder->id);
    sleep(coder->compile);
    printf("Coder id.%d is debug", coder->id);
    sleep(coder->debug);
    printf("Coder id.%d is refractor", coder->id);
    sleep(coder->refractor);
    return (NULL);
}
#include "../header/codexion.h"

int    create_thread(t_coder *anyone)
{
    if (pthread_create(&anyone->thread, NULL, &routine_coder, anyone) != 0)
        return 1;
    return 0;
}


int    close_thread(t_coder *anyone)
{
    pthread_join(anyone->thread, NULL);
    return 1;
}

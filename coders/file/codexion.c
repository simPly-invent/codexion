#include "../header/codexion.h"


int main(int argc, char **argv)
{
    int i;
    int size;
    int verif;
    bool   response;
    t_pars parsing;
    t_coder *coders;
    t_dongle *dongles;
    t_info_monitor info;
    t_simulation simu;
    pthread_t monitor;
    t_character *chara;

    i = 0;
    size = 0;
    verif = parser(argv, argc - 1, &parsing);
    if (verif < 0)
    {
        printf("\nAn error occurse\n");
        return 0;
    }   
    size = parsing.nbr_coder;
    coders = malloc(sizeof(t_coder) * size);
    if (!coders)
        return (0);
    dongles = malloc(sizeof(t_dongle) * size);
    if(!dongles)
    {
        free(coders);
        return 0;
    }
    chara = malloc(sizeof(t_character) * size);
    if (!chara)
    {
        free(coders);
        free(dongles);
        return (0);
    }
    init_simu(&simu);
    init_table(coders, &parsing, dongles, size);
    while (i < size)
    {
        init_character(&chara[i], &coders[i], &simu, dongles);
        i++;
    }
    init_monitor(&info, &parsing, chara, coders, size);
    response = create_thread_monitor(&info, &monitor);
    i = 0;
    while (i < size)
    {
        create_thread(&chara[i]);
        i++;
    }
    i = 0;
    while (i < size)
    {
        close_thread(&chara[i]);
        i++;
    }
    if (!response)
        close_thread_monitor(monitor);
    pthread_join(monitor, NULL);
    free(coders);
    return 0;
}

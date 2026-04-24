#include "../header/codexion.h"


int main(int argc, char **argv)
{
    int size;
    int verif;
    t_pars parsing;
    t_coder *coders;
    t_dongle *dongles;
    t_info_monitor *info;
    t_simulation *simu;

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
    init_table(coders, &parsing, dongles, size);
    init_simulation(info);
    init_monitor(info, &parsing, simu, coders);
    

    free(coders);
    return 0;
}

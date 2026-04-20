#include "../header/codexion.h"


int main(int argc, char **argv)
{
    int i;
    int size;
    int verif;
    int tmp;
    t_pars parsing;
    t_coder *coders;
    t_dongle *dongles;

    i = 0;
    size = 0;
    verif = parser(argv, argc - 1, &parsing);
    if (verif < 0)
    {
        printf("an error occurse");
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
    tmp = size;
    while(tmp > 0)
    {
        init_coder(&coders[i], &parsing, i + 1);
        i++;
        tmp--;
    }
    i = 0;
    while(i < size)
    {
        init_dongle(&dongles[i], &parsing, size);
        i++;
    }
    i = 0;
    while(i < size)
    {
        coders[i].left = &dongles[i];
        coders[i].right = &dongles[(i + 1) % size];
        i++;
    }
    // i = 0;
    // while (i < size)
    // {
    //     create_thread(&coders[i]);
    //     i++;   
    // }
    // i = 0;
    // while (i < size)
    // {
    //     close_thread(&coders[i]);
    //     i++;   
    // }
    free(coders);
    return 0;
}
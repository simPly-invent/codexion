#include "../header/codexion.h"



int *parser(char **argv, int size, t_pars *parsing)
{
    int i;
    int j;
    int *tab;

    if (size < 7)
        return (NULL);
    i = 1;
    j = 0;
    if(check_fifo_edf(argv[7]) == 1)
        return (NULL);
    tab = malloc(sizeof(int) * size);
    if (!tab)
        return (NULL);
    while(i <= size - 1)
    {
        tab[j] = atoi(argv[i]);
        i++;
        j++;
    }
    init_env(tab, parsing);
    printf("\033c");
    i = 0;
    while(i < 3)
    {
        printf(".");
        fflush(stdout);
        sleep(1);
        i++;
    }
    printf("\n[\033[32mOk\033[0m]: initialization de l'environement\n");
    return tab;
}
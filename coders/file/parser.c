#include "../header/codexion.h"



int parser(char **argv, int size, t_pars *parsing)
{
    int i;
    int j;
    int *tab;
    char *str;

    if (size < 7)
        return (-1);
    i = 1;
    j = 0;
    if(check_fifo_edf(argv[7]) == 1)
        return (-1);
    tab = malloc((sizeof(int) * size) - 1);
    str = ft_strdup(argv[size]);
    if (!tab)
        return (-1);
    while(i < size)
    {
        tab[j] = atoi(argv[i]);
        i++;
        j++;
    }
    init_pars(tab, parsing, str);
    free(tab);
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
    return 0;
}
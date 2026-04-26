#include "../header/codexion.h"



int parser(char **argv, int size, t_pars *parsing)
{
    int i;
    int j;
    int *tab;
    char *str;

    if (size < 8)
        return (-1);
    i = 1;
    j = 0;
    if(check_fifo_edf(argv[8]) == 1)
        return (-1);
    tab = malloc(sizeof(int) * size);
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
    loading_screen();
    printf("\n[\033[32mOk\033[0m]: initialization de l'environement\n");
    return 0;
}

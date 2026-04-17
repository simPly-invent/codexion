#include "../header/codexion.h"


int *parser(char **argv, int size, t_pars parsing)
{
    if (size < 7)
        return (NULL);
    int i = 1;
    int j = 0;
    int *tab = malloc(sizeof(int) * size);
    if (!tab)
    {
        return (NULL);        
    }
    while(i <= size)
    {
        tab[j] = atoi(argv[i]);
        i++;
        j++;
    }
    init_env(tab, parsing);
    return tab;
}
#include "../header/codexion.h"


int main(int argc, char **argv)
{
    int i;
    int size;
    int *tab;
    t_pars parsing;
    t_coder *coders;

    i = 0;
    size = 0;
    tab = parser(argv, argc - 1, &parsing);
    if(!tab)
    {
        printf("\n[\033[31mError\033[0m]: an error occurse\n");
    }
    size = parsing.nbr_coder;
    coders = malloc(sizeof(t_coder) * size);
    if (!coders)
    {
        free(tab);
        return (0);
    }
    while(size >= 0)
    {
        init_coder(&coders[i], &parsing, i);
        i++;
        size--;
    }
    free(coders);
    return 0;
}
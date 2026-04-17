#include "../header/codexion.h"


int main(int argc, char **argv)
{
    int i = 0;
    t_pars parsing;
    int *tab = parser(argv, argc - 1, parsing);
    if(!tab)
    {
        printf("[\033[31merror\033[0m]: to few argument");
    }
    return 0;
}
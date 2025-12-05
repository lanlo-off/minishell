#include "../../../includes/minishell.h"

bool ft_cd(void)
{
    char *pwd;

    pwd = getcwd(NULL, 0);
    printf("pwd: %s\n", pwd);
    chdir("/Users/maxime.m/Documents/CodingInC/");
    pwd = getcwd(NULL, 0);
    printf("pwd: %s\n", pwd);
    return (true);
    // compare la valeur au subtoken
    // jour la command
    // gestion erreur
    // retour si ok
}
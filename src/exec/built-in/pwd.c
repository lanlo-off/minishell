#include "../../../includes/minishell.h"

bool ft_pwd(void)
{
    char *cwd;
    
    cwd = getcwd(NULL, 0);
    if (!cwd)
	{
		ft_putstr_fd("pwd: error retrieving current directory\n", 2);
		return (EXIT_FAILURE);
	}
    else
    {
        ft_printf("%s\n", cwd);
    }
    return (true);
}
#include "../../../includes/minishell.h"

bool  ft_pwd(char **av) 
{
  char *cwd;
  int lenArgs;

  lenArgs = check_args(av, 1);
  cwd = getcwd(NULL, PATH_MAX);
  if (!cwd || lenArgs != 1) 
  {
    ft_putstr_fd("pwd: error retrieving current directory\n", 2);
    return (EXIT_FAILURE);
  }
  else 
    ft_printf("%s\n", cwd);
  return (true);
}

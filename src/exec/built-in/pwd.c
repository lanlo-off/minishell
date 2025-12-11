#include "../../../includes/minishell.h"

int  ft_pwd(void)
{
  char *cwd;

  cwd = getcwd(NULL, PATH_MAX);
  if (!cwd)
  {
		perror("minishell: pwd");
    return (EXIT_FAILURE);
  }
  else 
    ft_printf("%s\n", cwd);
  return (EXIT_SUCCESS);
}

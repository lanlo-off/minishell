#include "../../../includes/minishell.h"

int  ft_pwd(t_cmd *cmd)
{
  char *cwd;
(void) cmd;
  cwd = getcwd(NULL, PATH_MAX);
  if (!cwd)
  {
		perror("minishell: pwd");
    return (EXIT_FAILURE);
  }
  else
	ft_putendl_fd(cwd, 1); 
  free(cwd);
  return (EXIT_SUCCESS);
}

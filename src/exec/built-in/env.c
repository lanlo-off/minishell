#include "../../../includes/minishell.h"

//Check pas d'argument ou option

bool ft_env(t_shell *shell) 
{
  t_env *temp;
  int lenArgs;

  lenArgs = check_args(shell->cmds->av, 1);
  temp = shell->env;
  if (lenArgs == 1)
  {
     while (temp) 
    {
    printf("%s=%s\n", temp->key, temp->value);
    temp = temp->next;
    }
  }
  else
  {
    ft_putstr_fd("env: Too many arguments\n", 2);
    return (false);
  }
  return (true);
}

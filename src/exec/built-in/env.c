#include "../../../includes/minishell.h"

//Check pas d'argument ou option

int ft_env(t_cmd *cmd, t_shell *shell)
{
  t_env *temp;
  int lenArgs;

  lenArgs = check_args(cmd->av, 1);
  temp = shell->env;
  if (lenArgs == 1)
  {
     while (temp) 
    {
    ft_printf("%s=%s\n", temp->key, temp->value);
    temp = temp->next;
    }
  }
  else
  {
    ft_putstr_fd("env: Too many Arguments\n", 2);
    return (EXIT_FAILURE);
  }
  return (EXIT_SUCCESS);
}

#include "../../../includes/minishell.h"

static void remove_env_node(t_shell *shell, t_env *node) {
  if (node->prev)
    node->prev->next = node->next;
  else
    shell->env = node->next;
  if (node->next)
    node->next->prev = node->prev;
  free(node->key);
  free(node->value);
  free(node);
}

int ft_unset(t_cmd *cmd, t_shell *shell) 
{
  int i;
  t_env *current;
  int key_len;

  if (!cmd->av[1])
    return (EXIT_SUCCESS);
  i = 1;
  while (cmd->av[i]) {
    current = shell->env;
    key_len = ft_strlen(shell->cmds->av[i]);
    while (current) 
    {
      if (ft_strncmp(current->key, shell->cmds->av[i], key_len + 1) == 0)
      {
        remove_env_node(shell, current);
        break;
      }
      current = current->next;
    }
    i++;
  }
  return (EXIT_SUCCESS);
}

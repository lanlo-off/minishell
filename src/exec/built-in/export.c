#include "../../../includes/minishell.h"

static int is_valid_export_format(char *arg) {
  int i;

  i = 0;
  if (!ft_isalpha(arg[i]) && arg[i] != '_')
    return (0);
  i++;
  while (arg[i] && arg[i] != '=') {
    if (!ft_isalnum(arg[i]) && arg[i] != '_')
      return (0);
    i++;
  }
  if (arg[i] != '=')
    return (0);
  return (1);
}

static t_env *find_env_node(t_shell *shell, char *key) {
  t_env *current;
  int key_len;

  current = shell->env;
  key_len = ft_strlen(key);
  while (current) {
    if (ft_strncmp(current->key, key, key_len + 1) == 0)
      return (current);
    current = current->next;
  }
  return (NULL);
}

static void append_env_node(t_shell *shell, char *key, char *value) {
  t_env *new_node;
  t_env *last;

  new_node = malloc(sizeof(t_env));
  if (!new_node)
    return;
  new_node->key = key;
  new_node->value = value;
  new_node->next = NULL;
  new_node->prev = NULL;
  if (!shell->env) {
    shell->env = new_node;
    return;
  }
  last = shell->env;
  while (last->next)
    last = last->next;
  last->next = new_node;
  new_node->prev = last;
}

bool export_var(t_shell *shell, char *arg) {
  char *key;
  char *value;
  int i;
  t_env *node;

  if (!is_valid_export_format(arg))
    return (false);
  i = 0;
  while (arg[i] != '=')
    i++;
  key = ft_substr(arg, 0, i);
  value = ft_strdup(arg + i + 1);
  if (!key || !value)
    return (free(key), free(value), false);
  node = find_env_node(shell, key);
  if (node) {
    free(node->value);
    node->value = value;
    free(key);
  } else
    append_env_node(shell, key, value);
  return (true);
}

bool ft_export(t_shell *shell) {
  int i;

  if (!shell->cmds->av[1])
    return (true);
  i = 1;
  while (shell->cmds->av[i]) {
    export_var(shell, shell->cmds->av[i]);
    i++;
  }
  return (true);
}

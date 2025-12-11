#include "../../../includes/minishell.h"

void update_oldpwd(t_shell *shell) {
  t_env *temp;
  char *res;
  char *export_str;

  temp = shell->env;
  res = NULL;
  while (temp) {
    if (ft_strncmp(temp->key, "PWD", 4) == 0)
      res = temp->value;
    temp = temp->next;
  }
  if (!res)
    return; // PWD not set
  if (res) {
    export_str = ft_strjoin("OLDPWD=", res);
    if (export_str) {
      export_var(shell, export_str);
      free(export_str);
    }
  }
}

bool update_pwd(t_shell *shell) {
  char cwd[PATH_MAX];
  char *pwd;

  update_oldpwd(shell);
  if (getcwd(cwd, PATH_MAX) == NULL)
    return (false);
  pwd = ft_strjoin("PWD=", cwd);
  if (!pwd)
    return (false);
  export_var(shell, pwd);
  free(pwd);
  return (true);
}

bool ft_cd(t_shell *shell) {
  int cd;
  int lenargs;
  char *home_value;

  home_value = get_expanded_var("HOME", &shell->env, shell);
  lenargs = check_args(shell->cmds->av, 2);
  if (lenargs <= 0)
    return (free(home_value), false);
  if (lenargs == 1)
    cd = chdir(home_value);
  else
    cd = chdir(shell->cmds->av[1]);
  if (cd == -1) {
    perror("cd");
    free(home_value);
    return (false);
  }
  update_pwd(shell);
  free(home_value);
  return (true);
}
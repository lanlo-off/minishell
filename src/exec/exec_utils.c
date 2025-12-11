#include "../../includes/minishell.h"

bool is_builtin(char *cmd) 
{
  if (!cmd)
    return (false);
  if (ft_strncmp(cmd, "pwd", 4) == 0)
    return (true);
  if (ft_strncmp(cmd, "echo", 5) == 0)
    return (true);
  if (ft_strncmp(cmd, "cd", 3) == 0)
    return (true);
  if (ft_strncmp(cmd, "export", 6) == 0)
    return (true);
  if (ft_strncmp(cmd, "env", 4) == 0)
    return (true);
  if (ft_strncmp(cmd, "unset", 6) == 0)
    return (true);
  if (ft_strncmp(cmd, "exit", 5) == 0)
    return (true);
  return (false);
}

void exec_builtin(t_shell *shell) {
  if (ft_strncmp(shell->cmds->av[0], "pwd", 4) == 0)
    ft_pwd(shell->cmds->av);
  if (ft_strncmp(shell->cmds->av[0], "echo", 5) == 0)
    ft_echo(shell->cmds->av);
  if (ft_strncmp(shell->cmds->av[0], "cd", 3) == 0)
    ft_cd(shell);
  if (ft_strncmp(shell->cmds->av[0], "env", 4) == 0)
    ft_env(shell);
  if (ft_strncmp(shell->cmds->av[0], "export", 6) == 0)
    ft_export(shell);
  if (ft_strncmp(shell->cmds->av[0], "unset", 6) == 0)
    ft_unset(shell);
  if (ft_strncmp(shell->cmds->av[0], "exit", 5) == 0)
    ft_exit(shell);
}

/* static void exec_single_builtin(t_shell *shell) {
  int saved_stdin;
  int saved_stdout;

  saved_stdin = dup(STDIN_FILENO);
  saved_stdout = dup(STDOUT_FILENO);
  if (handle_redirections(shell->cmds))
    exec_builtin(shell);
  dup2(saved_stdin, STDIN_FILENO);
  dup2(saved_stdout, STDOUT_FILENO);
  close(saved_stdin);
  close(saved_stdout);
}

void exec_cmd(t_shell *shell) {
  if (!shell->cmds)
    return;
  if (!shell->cmds->next && shell->cmds->av && shell->cmds->av[0] &&
      is_builtin(shell->cmds->av[0]))
    exec_single_builtin(shell);
  else
    exec_pipeline(shell);
} */

#include "../../includes/minishell.h"

static void exec_child(t_cmd *cmd, int *p_fd, int fd_in, t_shell *shell) {
  (void)shell;
  if (fd_in != STDIN_FILENO) {
    dup2(fd_in, STDIN_FILENO);
    close(fd_in);
  }
  if (p_fd[1] != STDOUT_FILENO) {
    dup2(p_fd[1], STDOUT_FILENO);
    close(p_fd[1]);
  }
  if (p_fd[0] != -1)
    close(p_fd[0]);
  if (handle_redirections(cmd)) {
    if (cmd->av && cmd->av[0]) {
      if (is_builtin(cmd->av[0]))
        exec_builtin(shell);
      else if (access(cmd->av[0], X_OK) == 0)
        execve(cmd->av[0], cmd->av, NULL); // TODO: Pass envp
      else {
        ft_putstr_fd("minishell: command not found: ", 2);
        ft_putendl_fd(cmd->av[0], 2);
        exit(127);
      }
    }
  }
  exit(EXIT_FAILURE);
}

void exec_pipeline(t_shell *shell) {
  t_cmd *cmd;
  int p_fd[2];
  int fd_in;
  pid_t pid;

  cmd = shell->cmds;
  fd_in = STDIN_FILENO;
  while (cmd) {
    if (cmd->next) {
      if (pipe(p_fd) == -1) {
        perror("pipe");
        return;
      }
    } else {
      p_fd[0] = -1;
      p_fd[1] = STDOUT_FILENO;
    }
    pid = fork();
    if (pid == -1) {
      perror("fork");
      return;
    }
    if (pid == 0)
      exec_child(cmd, p_fd, fd_in, shell);
    if (fd_in != STDIN_FILENO)
      close(fd_in);
    if (p_fd[1] != STDOUT_FILENO)
      close(p_fd[1]);
    fd_in = p_fd[0];
    cmd = cmd->next;
  }
  while (wait(NULL) > 0)
    ;
}

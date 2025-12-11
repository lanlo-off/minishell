#include "../../includes/minishell.h"

static bool handle_redir_in(char *file) 
{
  int fd;

  fd = open(file, O_RDONLY);
  if (fd == -1) {
    perror(file);
    return (false);
  }
  if (dup2(fd, STDIN_FILENO) == -1) {
    perror("dup2");
    close(fd);
    return (false);
  }
  close(fd);
  return (true);
}

static bool handle_redir_out(char *file, bool append) 
{
  int fd;
  int flags;

  flags = O_WRONLY | O_CREAT;
  if (append)
    flags |= O_APPEND;
  else
    flags |= O_TRUNC;
  fd = open(file, flags, 0644);
  if (fd == -1) {
    perror(file);
    return (false);
  }
  if (dup2(fd, STDOUT_FILENO) == -1) {
    perror("dup2");
    close(fd);
    return (false);
  }
  close(fd);
  return (true);
}

bool handle_redirections(t_cmd *cmd) 
{
  t_redir *redir;

  redir = cmd->redirs;
  while (redir) {
    if (redir->type == REDIR_IN) {
      if (!handle_redir_in(redir->file))
        return (false);
    } else if (redir->type == REDIR_OUT) {
      if (!handle_redir_out(redir->file, false))
        return (false);
    } else if (redir->type == APPEND) {
      if (!handle_redir_out(redir->file, true))
        return (false);
    }
    redir = redir->next;
  }
  return (true);
}

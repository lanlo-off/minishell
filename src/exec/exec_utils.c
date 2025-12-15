#include "../../includes/minishell.h"

void close_fds(int fd_in, int fd_out)
{
	if (fd_in >= 0 && !is_std_fd(fd_in))
		close(fd_in);
	if (fd_out >= 0 && is_std_fd(fd_out))
		close(fd_out);
}

bool is_builtin(t_cmd *cmd)
{
	if (!cmd)
		return (false);
	if (ft_strncmp(cmd->av[0], "pwd", 4) == 0)
		return (true);
	if (ft_strncmp(cmd->av[0], "echo", 5) == 0)
		return (true);
	if (ft_strncmp(cmd->av[0], "cd", 3) == 0)
		return (true);
	if (ft_strncmp(cmd->av[0], "export", 6) == 0)
		return (true);
	if (ft_strncmp(cmd->av[0], "env", 4) == 0)
		return (true);
	if (ft_strncmp(cmd->av[0], "unset", 6) == 0)
		return (true);
	if (ft_strncmp(cmd->av[0], "exit", 5) == 0)
		return (true);
	return (false);
}

int exec_builtin(t_cmd *cmd, t_shell *shell)
{
	if (ft_strncmp(shell->cmds->av[0], "pwd", 4) == 0)
		return (ft_pwd());
	if (ft_strncmp(shell->cmds->av[0], "echo", 5) == 0)
		return (ft_echo(cmd->av));
	if (ft_strncmp(shell->cmds->av[0], "cd", 3) == 0)
		return (ft_cd(cmd, shell));
	if (ft_strncmp(shell->cmds->av[0], "env", 4) == 0)
		return (ft_env(cmd, shell));
	if (ft_strncmp(shell->cmds->av[0], "export", 6) == 0)
		return (ft_export(cmd, shell));
	if (ft_strncmp(shell->cmds->av[0], "unset", 6) == 0)
		return (ft_unset(cmd, shell));
	if (ft_strncmp(shell->cmds->av[0], "exit", 5) == 0)
		return (ft_exit(cmd, shell));
	return (false);
}

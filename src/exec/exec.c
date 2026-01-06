/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:55:40 by llechert          #+#    #+#             */
/*   Updated: 2026/01/06 14:08:37 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	set_normal_fds(t_cmd *cmd, int pipefd[2])
{
	if (!cmd->prev && !cmd->next)
	{
		cmd->fd_in = STDIN_FILENO;
		cmd->fd_out = STDOUT_FILENO;
	}
	if (cmd->next)
	{
		if (pipe(pipefd) == -1)
		{
			print_error(NULL, errno, ERR_PIPE, cmd);
			return (false);
		}
		if (cmd->prev)
			cmd->fd_in = cmd->prev->fd_out;
		else
			cmd->fd_in = STDIN_FILENO;
		cmd->fd_out = pipefd[1];
	}
	else if (cmd->prev)
	{
		cmd->fd_in = cmd->prev->fd_out;
		cmd->fd_out = STDOUT_FILENO;
	}
	return (true);
}

void	exec_cmd(t_cmd *cmd, char **envp, t_shell *shell)
{
	if (cmd->av && cmd->av[0] && !cmd->av[0][0])
	{
		print_error("", 0, ERR_CMD_NOT_FOUND, cmd);
		exit_fork(cmd, shell);
	}
	if (is_builtin(cmd))
	{
		cmd->exit_status = exec_builtin(cmd, shell);
		exit_fork(cmd, shell);
	}
	execve(cmd->path, cmd->av, envp);
	print_error(cmd->path, errno, ERR_EXEC, cmd);
	exit_fork(cmd, shell);
}

bool	do_cmd(t_cmd *cmd, t_shell *shell, int pipefd[2])
{
	g_signal_received = 2;
	cmd->pid = fork();
	if (cmd->pid == -1)
		return (print_error(NULL, errno, ERR_FORK, cmd), false);
	else if (cmd->pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		(dup2(cmd->fd_in, STDIN_FILENO), dup2(cmd->fd_out, STDOUT_FILENO));
		close_fds_ptr(&pipefd[0], &pipefd[1]);
		close_fds_ptr(&cmd->fd_in, &cmd->fd_out);
		close_hd_fds(shell);
		exec_cmd(cmd, shell->envp, shell);
		return (false);
	}
	else
	{
		close_fds_ptr(&pipefd[1], NULL);
		if (cmd->prev && cmd->prev->fd_out != STDOUT_FILENO)
			close_fds_ptr(&cmd->prev->fd_out, NULL);
		close_fds_ptr(&cmd->fd_in, NULL);
		if (cmd->next)
			cmd->fd_out = pipefd[0];
		return (true);
	}
}

static bool	handle_single_cmd(t_cmd *cmd, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = 0;
	saved_stdout = 0;
	if (!cmd)
		return (true);
	if (!handle_fds_single_cmd(cmd))
		return (false);
	if (!is_builtin(cmd))
		return (fork_single_cmd(cmd, shell));
	else
		single_builtin(cmd, saved_stdin, saved_stdout, shell);
	return (cmd->exit_status == 0);
}

bool	execution(t_shell *shell, t_cmd *cmd_lst)
{
	t_cmd	*cmd;
	int		pipefd[2];

	if (!cmd_lst)
		return (true);
	cmd = cmd_lst;
	init_value(&pipefd[0], &pipefd[1]);
	if (!create_all_heredocs(shell, cmd_lst))
		return (false);
	if (!cmd->next && !cmd->prev)
		return (handle_single_cmd(cmd, shell));
	while (cmd)
	{
		if (!set_normal_fds(cmd, pipefd))
			return (false);
		if (!handle_redir_in(cmd, cmd->redirs_in)
			|| !handle_redir_out(cmd, cmd->redirs_out) || !check_cmd(cmd)
			|| !do_cmd(cmd, shell, pipefd))
		{
			cmd = cmd->next;
			continue ;
		}
		cmd = cmd->next;
	}
	return (close_fds_ptr(&pipefd[0], &pipefd[1]), true);
}

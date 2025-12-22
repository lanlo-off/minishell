/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:14:26 by llechert          #+#    #+#             */
/*   Updated: 2025/12/22 18:32:48 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_std_fd(int fd)
{
	if (fd == STDIN_FILENO)
		return (true);
	if (fd == STDOUT_FILENO)
		return (true);
	if (fd == STDERR_FILENO)
		return (true);
	return (false);
}

int	open_outfile(char *file, t_token_type type, t_cmd *cmd)
{
	int	fd;

	fd = -1;
	if (type == REDIR_OUT)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		print_error(file, errno, ERR_REDIR, cmd);
	return (fd);
}

bool	handle_redir_out(t_cmd *cmd, t_redir *redir_lst)
{
	t_redir	*redir;

	if (!redir_lst)
		return (true);
	redir = redir_lst;
	while (redir)
	{
		if (cmd->fd_out >= 0 && !is_std_fd(cmd->fd_out))
			close_fds_ptr(&cmd->fd_out, NULL);
		cmd->fd_out = open_outfile(redir->file, redir->type, cmd);
		if (cmd->fd_out < 0)
			return (false);
		redir = redir->next;
	}
	return (true);
}

int	open_infile(char *file, t_cmd *cmd)
{
	int	fd;

	fd = -1;
	fd = open(file, O_RDONLY);
	if (fd < 0)
		print_error(file, errno, ERR_REDIR, cmd);
	return (fd);
}

bool	handle_redir_in(t_cmd *cmd, t_redir *redir_lst, t_shell *shell, int pipefd[2])
{
	t_redir	*redir;

	if (!cmd || !redir_lst)
		return (true);
	redir = redir_lst;
	while (redir)
	{
		if (cmd->fd_in >= 0 && !is_std_fd(cmd->fd_in))
			close_fds_ptr(&cmd->fd_in, NULL);
		if (redir->type == HEREDOC && !create_heredoc(cmd, redir, shell, pipefd))
		{
			if (cmd->exit_status == 0)
				cmd->exit_status = 1;
			return (false);
		}
		else if (redir->type == REDIR_IN)
		{
			cmd->fd_in = open_infile(redir->file, cmd);
			if (cmd->fd_in < 0)
				return (false);
		}
		redir = redir->next;
	}
	return (true);
}

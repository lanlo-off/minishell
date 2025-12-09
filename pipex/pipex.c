/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 13:26:08 by llechert          #+#    #+#             */
/*   Updated: 2025/12/09 19:00:34 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	pipex(t_cmd *tab_cmds, t_args *args)
{
	int	i;
	int	pipefd[2];

	i = 0;
	if (args->hd)
		handle_here_doc(tab_cmds, args);
	while (i < args->nb_cmd)
	{
		renew_pipe(tab_cmds, i, pipefd, args);
		do_cmd(tab_cmds, i, pipefd, args);
		i++;
	}
}

void	renew_pipe(t_cmd *tab_cmds, int i, int pipefd[2], t_args *args)
{
	if (i < args->nb_cmd - 1)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			exit_error(tab_cmds, args, args->nb_cmd, 1);
		}
		if (i > 0)
			tab_cmds[i].fd_in = tab_cmds[i - 1].fd_out;
		tab_cmds[i].fd_out = pipefd[1];
	}
	else
		tab_cmds[i].fd_in = tab_cmds[i - 1].fd_out;
}

void	do_cmd(t_cmd *tab_cmds, int i, int pipefd[2], t_args *args)
{
	tab_cmds[i].pid = fork();
	if (tab_cmds[i].pid == -1)
		exit_bad_fork(tab_cmds, args);
	else if (tab_cmds[i].pid == 0)
	{
		if (i < args->nb_cmd - 1 && tab_cmds[args->nb_cmd - 1].fd_out >= 0)
			close(tab_cmds[args->nb_cmd - 1].fd_out);
		dup2_fds(tab_cmds[i].fd_in, tab_cmds[i].fd_out);
		close_fds(pipefd[0], pipefd[1]);
		close_fds(tab_cmds[i].fd_in, tab_cmds[i].fd_out);
		exec_cmd(tab_cmds, i, args);
	}
	else
	{
		close(pipefd[1]);
		if (i > 0 && tab_cmds[i - 1].fd_out != STDOUT_FILENO)
			close(tab_cmds[i - 1].fd_out);
		if (tab_cmds[i].fd_in >= 0 && tab_cmds[i].fd_in != STDIN_FILENO)
			close(tab_cmds[i].fd_in);
		if (i < args->nb_cmd - 1)
			tab_cmds[i].fd_out = pipefd[0];
	}
}

void	exec_cmd(t_cmd *tab_cmds, int i, t_args *args)
{
	if (!tab_cmds[i].cmd_split)
	{
		perror("Could not split cmd");
		exit_error(tab_cmds, args, args->nb_cmd, 1);
	}
	if (!tab_cmds[i].cmd_split[0])
	{
		perror("Command empty");
		exit_error(tab_cmds, args, args->nb_cmd, 127);
	}
	if (!tab_cmds[i].path)
	{
		ft_putendl_fd("path not found", 2);
		exit_error(tab_cmds, args, args->nb_cmd, 127);
	}
	if (tab_cmds[i].fd_in >= 0 && tab_cmds[i].fd_out >= 0)
		execve(tab_cmds[i].path, tab_cmds[i].cmd_split, args->env);
	perror("execve");
	exit_error(tab_cmds, args, args->nb_cmd, 127);
}

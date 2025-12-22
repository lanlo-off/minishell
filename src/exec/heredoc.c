/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 11:32:32 by llechert          #+#    #+#             */
/*   Updated: 2025/12/22 19:39:11 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*expand_heredoc(char *line, t_sub_type quote_type, t_shell *shell)
{
	int		i;
	char	*res;
	char	*chunk;

	if (quote_type != SUB_UNQUOTED)
		return (ft_strdup(line));
	i = 0;
	res = ft_strdup("");
	while (line[i])
	{
		res = append_until_doll(line, &i, res);
		if (!line[i])
			break ;
		i++;
		chunk = expand_var(line, &i, shell);
		res = join_and_free(res, chunk);
	}
	return (res);
}

static bool	clear_heredoc(t_shell *shell, int *pipe_write_end, int exitcode, int pipefd[2])
{
	if (pipefd)
		close_fds_ptr(&pipefd[0], &pipefd[1]);
	close_fds_ptr(pipe_write_end, NULL);
	clean_post_parser(shell);
	clean_shell(shell);
	exit(exitcode);
}

static int	check_hd(char *line, t_redir *redir)
{
	if (!line)
	{
		ft_putendl_fd("minishell: warning: here-document delimited \
					by end-of-file", 2);
		return (1);
	}
	if (ft_strcmp(line, redir->file) == 0)
	{
		free(line);
		return (2);
	}
	return (0);
}

static void	hdoc_loop(int pipe_write_end, t_redir *redir, t_shell *shell, int pipefd[2])
{
	char	*line;
	char	*exp_line;

	(signal(SIGINT, heredoc_sigint_handler), signal(SIGQUIT, SIG_IGN));
	rl_event_hook = rl_sigint_hook;
	while (true)
	{
		line = readline(">");
		if (g_signal_received == 67)
			(free(line), clear_heredoc(shell, &pipe_write_end, 130, pipefd));
		if (check_hd(line, redir) != 0)
			break ;
		exp_line = expand_heredoc(line, redir->file_quote_type, shell);
		free(line);
		if (!exp_line)
			exit(EXIT_FAILURE);
		(ft_putendl_fd(exp_line, pipe_write_end), free(exp_line));
	}
	clear_heredoc(shell, &pipe_write_end, 1, pipefd);
}

bool	create_heredoc(t_cmd *cmd, t_redir *redir, t_shell *shell, int pipefd[2])
{
	int		pipefd_hd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipefd_hd) == -1)
		return (print_error(NULL, errno, ERR_PIPE, cmd), false);
	pid = fork();
	if (pid == -1)
		return (print_error(NULL, errno, ERR_FORK, cmd),
			close_fds_ptr(&pipefd_hd[0], &pipefd_hd[1]), false);
	if (pid == 0)
		(close_fds_ptr(&pipefd_hd[0], NULL), hdoc_loop(pipefd_hd[1], redir, shell, pipefd));
	close_fds_ptr(&pipefd_hd[1], NULL);
	(signal(SIGINT, SIG_IGN), waitpid(pid, &status, 0), init_signals());
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		return (cmd->exit_status = 130, close_fds_ptr(&pipefd_hd[0], NULL), false);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		cmd->exit_status = 130;
		g_signal_received = 130;
		return (close_fds_ptr(&pipefd_hd[0], NULL), false);
	}
	cmd->fd_in = pipefd_hd[0];
	return (true);
}

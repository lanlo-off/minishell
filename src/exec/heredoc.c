/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 11:32:32 by llechert          #+#    #+#             */
/*   Updated: 2026/01/05 16:09:58 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief On expand si type est unquoted uniquement
 * On reutilise la structure et les fonctions du parser
 *
 * @param line
 * @param quote_type
 * @param shell
 * @return char*
 */
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

static bool	clear_heredoc(t_shell *shell, int *fd_out, int exitcode)
{
	close_fds_ptr(fd_out, NULL);
	close_hd_fds(shell);
	clean_post_parser(shell);
	clean_shell(shell);
	exit(exitcode);
}

static void	heredoc_loop(int fd_out, t_redir *redir, t_shell *shell)
{
	char	*line;
	char	*exp_line;

	(signal(SIGINT, heredoc_sigint_handler), signal(SIGQUIT, SIG_IGN));
	rl_event_hook = rl_sigint_hook;
	while (true)
	{
		line = readline("> ");
		if (g_signal_received == 67)
			(free(line), clear_heredoc(shell, &fd_out, 130));
		if (check_hd(line, redir) != 0)
			break ;
		exp_line = expand_heredoc(line, redir->file_quote_type, shell);
		free(line);
		if (!exp_line)
			exit(EXIT_FAILURE);
		(ft_putendl_fd(exp_line, fd_out), free(exp_line));
	}
	clear_heredoc(shell, &fd_out, 1);
}

static bool	create_hd(t_cmd *cmd, t_redir *redir, t_shell *shell)
{
	int		fd;
	pid_t	pid;
	int		status;

	fd = open(HD_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (print_error(HD_FILE, errno, ERR_REDIR, cmd), false);
	pid = fork();
	if (pid == -1)
		return (close(fd), print_error(NULL, errno, ERR_FORK, cmd), false);
	if (pid == 0)
		heredoc_loop(fd, redir, shell);
	close(fd);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	init_signals();
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		return (unlink(HD_FILE), cmd->exit_status = 130, false);
	redir->hd_fd = open(HD_FILE, O_RDONLY);
	unlink(HD_FILE);
	if (redir->hd_fd == -1)
		return (print_error(HD_FILE, errno, ERR_REDIR, cmd), false);
	return (true);
}

bool	create_all_heredocs(t_shell *shell, t_cmd *cmd_lst)
{
	t_cmd	*cmd;
	t_redir	*redir;

	cmd = cmd_lst;
	while (cmd)
	{
		if (cmd->redirs_in)
		{
			redir = cmd->redirs_in;
			while (redir)
			{
				if (redir->type == HEREDOC && !create_hd(cmd, redir, shell))
				{
					if (cmd->exit_status == 0)
						cmd->exit_status = 1;
					return (false);
				}
				redir = redir->next;
			}
		}
		cmd = cmd->next;
	}
	return (true);
}

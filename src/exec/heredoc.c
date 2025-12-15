/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 11:32:32 by llechert          #+#    #+#             */
/*   Updated: 2025/12/15 15:19:52 by llechert         ###   ########.fr       */
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
static char *expand_heredoc(char *line, t_sub_type quote_type, t_shell *shell)
{
	int i;
	char *res;
	char *chunk;

	if (quote_type != SUB_UNQUOTED)
		return (ft_strdup(line));
	i = 0;
	res = ft_strdup("");
	while (line[i])
	{
		res = append_until_doll(line, &i, res);
		if (!line[i])
			break;
		i++;
		chunk = expand_var(line, &i, shell); // donc faut recuperer le shell
		res = join_and_free(res, chunk);
	}
	return (res);
}

/**
 * @brief Create a heredoc pipe (permet de gerer jusqu'a 1MB de char donc
 * suffisant, pas besoin de creer un doc ?!) Ecris ce qui arrive du here_doc
 * dans le pipefd[1] (cote ecriture) Attribue pipefd[0] en fd_in de la commande
 * en question S'il y a plusieurs here_doc successifs, le fd_in de la commande
 * sera celui du dernier here_doc Verifier capacite de gestion du pipe : cat
 * /proc/sys/fs/pipe-max-size
 *
 * @param cmd
 * @param redir
 * @return true
 * @return false
 */
static void heredoc_loop(int pipe_write_end, t_redir *redir, t_shell *shell)
{
	char *line;
	char *exp_line;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	while (true)
	{
		line = readline(">");
		if (!line)
		{
			ft_putendl_fd("minishell: warning: here-document delimited by end-of-file", 2);
			break;
		}
		if (ft_strcmp(line, redir->file) == 0)
		{
			free(line);
			break;
		}
		exp_line = expand_heredoc(line, redir->file_quote_type, shell);
		free(line);
		if (!exp_line)
			exit(EXIT_FAILURE);
		ft_putendl_fd(exp_line, pipe_write_end);
		free(exp_line);
	}
	close_fds(&pipe_write_end, NULL);
	exit(EXIT_SUCCESS);
}

bool create_heredoc(t_cmd *cmd, t_redir *redir, t_shell *shell)
{
	int pipefd[2];
	pid_t pid;
	int status;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), false);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), close_fds(&pipefd[0], &pipefd[1]), false);
	if (pid == 0)
	{
		close_fds(&pipefd[0], NULL);
		heredoc_loop(pipefd[1], redir, shell);
	}
	close_fds(&pipefd[1], NULL);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	init_signals();
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		return (close_fds(&pipefd[0], NULL), false);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		g_signal_received = 130;
		return (close_fds(&pipefd[0], NULL), false);
	}
	cmd->fd_in = pipefd[0];
	return (true);
}

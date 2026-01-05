/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 11:32:32 by llechert          #+#    #+#             */
/*   Updated: 2026/01/05 15:16:02 by llechert         ###   ########.fr       */
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
static void heredoc_loop(int fd_out, t_redir *redir, t_shell *shell)
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

static bool create_heredoc(t_cmd *cmd, t_redir *redir, t_shell *shell)
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
	close(fd); // On ferme l'accès en écriture (on va l'ouvrir en lecture bientot)
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	init_signals();
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		return (unlink(HD_FILE), cmd->exit_status = 130, false);
	redir->hd_fd = open(HD_FILE, O_RDONLY);
	unlink(HD_FILE);// 3. IMPORTANT : On unlink immédiatement mais le fichier est marqué pour suppression, mais reste accessible tant que redir->hd_fd est ouvert. On le closera plus tard dans la loop de redirection ou apres la commande
	if (redir->hd_fd == -1)
		return (print_error(HD_FILE, errno, ERR_REDIR, cmd), false);
	return (true);
}

bool	create_all_heredocs(t_shell *shell, t_cmd *cmd_lst)
{
	t_cmd	*cmd;
	t_redir	*redir_tmp;

	cmd = cmd_lst;
	while (cmd)
	{
		if (cmd->redirs_in)
		{
			redir_tmp = cmd->redirs_in;
			while (redir_tmp)
			{
				if (redir_tmp->type == HEREDOC && !create_heredoc(cmd, redir_tmp, shell))
				{
					if (cmd->exit_status == 0)
						cmd->exit_status = 1;
					return (false);
				}
				redir_tmp = redir_tmp->next;
			}
		}
		cmd = cmd->next;
	}
	return (true);
}

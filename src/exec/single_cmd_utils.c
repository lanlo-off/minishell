/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 13:14:29 by llechert          #+#    #+#             */
/*   Updated: 2025/12/15 13:39:53 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Pour verifier si on l'envoie a execve et sinon imprimer le message
 * d'erreur
 *
 * @param cmd
 * @return true
 * @return false
 */
bool	check_cmd(t_cmd *cmd)
{
	if (!cmd->av)
	{
		perror("Could not split cmd");
		return (false);
	}
	if (!cmd->av[0])
	{
		perror("Command empty");
		return (false);
	}
	if (!cmd->path)
	{
		ft_putendl_fd("path not found", 2);
		return (false);
	}
	return (true);
}

bool	handle_fds_single_cmd(t_cmd *cmd, t_shell *shell)
{
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	if (!handle_redir_in(cmd, cmd->redirs_in, shell))
		return (false);
	if (!handle_redir_out(cmd, cmd->redirs_out))
		return (false);
	return (true);
}

bool	fork_single_cmd(t_cmd *cmd, t_shell *shell)
{
	cmd->pid = fork();
	if (cmd->pid == -1)
		return (false); // exit_bad_fork(cmd), quel comportement ? Fin de la commande et on continue ? Fin de la chaine de cmd ?
	else if (cmd->pid == 0)
	{
		(dup2(cmd->fd_in, STDIN_FILENO), dup2(cmd->fd_out, STDOUT_FILENO));
		if (check_cmd(cmd))
			exec_cmd(cmd, shell->envp, shell);
		return (false); // car check_cmd ou execve a fail
	}
	else
	{
		if (cmd->prev && cmd->prev->fd_out != STDOUT_FILENO)
			close(cmd->prev->fd_out);
		if (cmd->fd_in >= 0 && cmd->fd_in != STDIN_FILENO)
			close(cmd->fd_in);
		return (true);
	}
}

/**
 * @brief Afin de faire un unique builtin il faut
 * stocker les STD(dup), puis dup2, puis executer,
 * puis restaurer(dup2), puis fermer
 * 
 * @param cmd 
 * @param saved_stdin 
 * @param saved_stdout 
 * @param shell 
 */
void	single_builtin(t_cmd *cmd, int saved_in, int saved_out, t_shell *shell)
{
	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (cmd->fd_in >= 0)
		dup2(cmd->fd_in, STDIN_FILENO);
	if (cmd->fd_out >= 0)
		dup2(cmd->fd_out, STDOUT_FILENO);
	cmd->exit_status = exec_builtin(cmd, shell);
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
	if (cmd->fd_in >= 0 && cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	if (cmd->fd_out >= 0 && cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
}
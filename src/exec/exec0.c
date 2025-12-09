/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec0.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:55:40 by llechert          #+#    #+#             */
/*   Updated: 2025/12/09 22:29:55 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	set_normal_fds(t_cmd *cmd, int pipefd[2])
{
	if (cmd->next)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			return (false);
		}
		if (cmd->prev)//si pas la premiere cmd
			cmd->fd_in = cmd->prev->fd_out;
		cmd->fd_out = pipefd[1];
	}
	else
		cmd->fd_in = cmd->prev->fd_out;
	return (true);
}

bool	execution(t_shell *shell, t_cmd *cmd_lst, t_env *env)
{
	t_cmd	*cmd;
	int		pipefd[2];
	
	if (!cmd_lst)
		return (true);
	cmd = cmd_lst;
	while (cmd)
	{
		if (!set_normal_fds(cmd, pipefd))
			return (false) ;// car faut pas faire les autres commandes quand on a fail le pipe ?
		if (!handle_redir_in(cmd, cmd->redirs_in))//Manage Heredoc ici et impression du message d'erreur si necessaire
			continue ;// ou return (false) ?
		if (!handle_redir_out(cmd, cmd->redirs_out))
			continue ;// ou return (false) ?
		//Si juste des redir et pas de cmd -> gerer les redir (HD et creation des outfile) et ne rien faire ensuite
		if (!do_cmd(cmd, env))
			continue ;// ou return (false) ?
		cmd = cmd->next;
	}
	return (true);
}

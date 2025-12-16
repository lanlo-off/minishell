/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:39:38 by llechert          #+#    #+#             */
/*   Updated: 2025/12/16 22:27:31 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// void	init_signals(shell)
// {

// }

void	init_structs(t_shell *shell, char **envp)
{
	if (!shell)
		return ;
	ft_bzero(shell, sizeof(t_shell));
	shell->env = get_env(envp);
	shell->envp = envp;
	init_signals();
}

bool	fill_cmd(t_cmd *cmd, t_shell *shell)
{
	if (!cmd)
		return (false);
	cmd->path_to_free = false;
	cmd->exit_status = 0;
	if (cmd->av && cmd->av[0])
	{
	if (is_builtin(cmd))
		cmd->path = cmd->av[0];
	else if (!ft_strchr(cmd->av[0], '/'))//si on ne trouve pas de / dans le nom de la commande -> on n'est pas en chemin absolu
	{
		cmd->path = get_path(cmd->av[0], shell->env);
		cmd->path_to_free = true;
	}
	else//si on nous a donne le chemin absolu (a verifier car peut etre que ca fonctionne pas si la personne envoie ../../(jusqu'a arriver au home)/user/bin/cat)
		cmd->path = cmd->av[0];
	}
	cmd->fd_in = -1;
	cmd->fd_out = -1;
	cmd->next = NULL;
	cmd->prev = NULL;
	cmd->pid = -1;
	return (true);
}

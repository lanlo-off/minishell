/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 13:30:55 by llechert          #+#    #+#             */
/*   Updated: 2025/12/17 18:05:29 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	clean_redir_cmd(t_redir *redirs)
{
	t_redir	*tmp;

	if (!redirs)
		return ;
	while (redirs->next)
	{
		tmp = redirs;
		redirs = redirs->next;
		free(tmp->file);
		free(tmp);
	}
	free(redirs->file);
	free(redirs);
}

static void	clean_args_cmd(char **av)
{
	int	i;

	i = 0;
	if (!av)
		return ;
	while (av[i])
	{
		free(av[i]);
		i++;
	}
	free(av[i]);
	free(av);
}

void	clean_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->av)
		clean_args_cmd(cmd->av);
	if (cmd->redirs_in)
		clean_redir_cmd(cmd->redirs_in);
	if (cmd->redirs_out)
		clean_redir_cmd(cmd->redirs_out);
	if (cmd->path_to_free)
		free(cmd->path);
	close_fds_ptr(&cmd->fd_in, &cmd->fd_out);
}

static void	clean_cmd_lst(t_cmd *cmd_lst)
{
	t_cmd	*tmp;

	if (!cmd_lst)
		return ;
	while (cmd_lst)
	{
		tmp = cmd_lst;
		clean_cmd(tmp);
		cmd_lst = cmd_lst->next;
		free(tmp);
	}
}

void	clean_post_parser(t_shell *shell)
{
	if (!shell)
		return ;
	clean_post_lexer(shell);
	if (shell->cmds)
		clean_cmd_lst(shell->cmds);
	shell->cmds = NULL;
}

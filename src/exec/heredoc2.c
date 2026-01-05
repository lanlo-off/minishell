/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 11:32:32 by llechert          #+#    #+#             */
/*   Updated: 2026/01/05 15:22:11 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_hd_fds(t_shell *shell)
{
	t_cmd	*cmd_tmp;
	t_redir	*redir_tmp;

	if (!shell->cmds)
		return ;
	cmd_tmp = shell->cmds;
	while (cmd_tmp)
	{
		if (cmd_tmp->redirs_in)
		{
			redir_tmp = cmd_tmp->redirs_in;
			while (redir_tmp)
			{
				close_fds_ptr(&redir_tmp->hd_fd, NULL);//qui est a -1 si pas un heredoc ou pas encore assigne
				redir_tmp = redir_tmp->next;
			}
		}
		cmd_tmp = cmd_tmp->next;
	}
}

int	check_hd(char *line, t_redir *redir)
{
	if (!line)
	{
		ft_putstr_fd("minishell: warning: here-document delimited", 2);
		ft_putendl_fd("by end-of-file", 2);
		return (1);
	}
	if (ft_strcmp(line, redir->file) == 0)
	{
		free(line);
		return (2);
	}
	return (0);
}

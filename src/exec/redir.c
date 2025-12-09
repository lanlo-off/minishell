/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:14:26 by llechert          #+#    #+#             */
/*   Updated: 2025/12/09 21:01:36 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	open_infile(char *file)
{
	int	fd;

	fd = -1;
	fd = open(file, O_RDONLY);
	if (fd < 0)
		ft_putstr_fd("PAS REUSSI A LIRE L'INFILE\n", 2);
	return (fd);
}

bool	handle_redir_in(t_cmd *cmd, t_redir *redir_lst, int pipefd[2])
{
	t_redir	*redir;

	if (!redir_lst)
		return (true);
	redir = redir_lst;
	while (redir)
	{
		if (redir->type == HEREDOC && !handle_heredoc(cmd, redir, pipefd))//on change le cmd->fd_in dans la fonction si erreur heredoc on l'ecrit ici
			return (false);
		else if (redir->type == REDIR_IN)
		{
			cmd->fd_in = open_infile(redir->file);
			if (cmd->fd_in < 0)
				return (false);
		}
		redir = redir->next;
	}
	return (true);
}

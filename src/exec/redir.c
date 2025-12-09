/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:14:26 by llechert          #+#    #+#             */
/*   Updated: 2025/12/09 22:07:53 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_std_fd(int fd)
{
	if (fd == STDIN_FILENO)
		return (true);
	if (fd == STDOUT_FILENO)
		return (true);
	if (fd == STDERR_FILENO)
		return (true);
	return (false);
}

int	open_outfile(char *file, t_token_type type)
{
	int	fd;

	fd = -1;
	if (type == REDIR_OUT)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);//normalement ca ouvre 0666 7 umask mais umask c'est quasi tout le temps 0022
	else
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		ft_putstr_fd("PAS REUSSI A LIRE L'INFILE!\n", 2);
	return (fd);
}

bool	handle_redir_out(t_cmd *cmd, t_redir *redir_lst)
{
	t_redir	*redir;

	if (!redir_lst)
		return (true);
	redir = redir_lst;
	while (redir)
	{
		if (cmd->fd_out >= 0 && !is_std_fd(cmd->fd_out))//si on a deja un fd ouvert mais qu'on va faire une redir, on peut le fermer il sert a rien
			close(cmd->fd_out);
		cmd->fd_out = open_outfile(redir->file, redir->type);
		if (cmd->fd_out < 0)
			return (false);
		redir = redir->next;
	}
	return (true);
}

int	open_infile(char *file)
{
	int	fd;

	fd = -1;
	fd = open(file, O_RDONLY);
	if (fd < 0)
		ft_putstr_fd("PAS REUSSI A LIRE L'INFILE\n", 2);
	return (fd);
}

bool	handle_redir_in(t_cmd *cmd, t_redir *redir_lst)
{
	t_redir	*redir;

	if (!redir_lst)
		return (true);
	redir = redir_lst;
	while (redir)
	{
		if (cmd->fd_in >= 0 && !is_std_fd(cmd->fd_in))//si on a deja un fd ouvert mais qu'on va faire une redir, on peut le fermer il sert a rien
			close(cmd->fd_in);
		if (redir->type == HEREDOC && !handle_heredoc(cmd, redir))//on change le cmd->fd_in dans la fonction si erreur heredoc on l'ecrit ici
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

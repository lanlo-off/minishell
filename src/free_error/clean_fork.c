/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_fork.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 21:58:15 by llechert          #+#    #+#             */
/*   Updated: 2025/12/17 16:51:05 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_fork(t_cmd *cmd, t_shell *shell)
{
	int	code;

	code = cmd->exit_status;
	// close_fds(cmd->fd_in, cmd->fd_out);
	close_fds_ptr(&cmd->fd_in, &cmd->fd_out);
	clean_post_parser(shell);
	clean_shell(shell);
	exit(code);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:32:10 by llechert          #+#    #+#             */
/*   Updated: 2025/12/16 17:34:22 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	print_error_parser(char *token, t_shell *shell)
{
	ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
	ft_putendl_fd(token, 2);
	shell->exit_code = 2;
	return (2);
}

int	print_error1(char *name, int err, t_err_type type, t_cmd *cmd)
{
	if (type == ERR_CMD_NOT_FOUND)
	{
		ft_putstr_fd(name, 2);
		ft_putendl_fd(": command not found", 2);
		cmd->exit_status = 127;
	}
	if (type == ERR_PIPE)
	{
		ft_putstr_fd("pipe: ", 2);
		ft_putendl_fd(strerror(err), 2);
		cmd->exit_status = 1;
	}
	if (type == ERR_FORK)
	{
		ft_putstr_fd("fork: ", 2);
		ft_putendl_fd(strerror(err), 2);
		cmd->exit_status = 1;
	}
	return (cmd->exit_status);
}

int	print_error(char *name, int err, t_err_type type, t_cmd *cmd)
{
	cmd->exit_status = 1;
	ft_putstr_fd("minishell: ", 2);
	if (type == ERR_CMD_NOT_FOUND || type == ERR_PIPE || type == ERR_FORK)
		return (print_error1(name, err, type, cmd));
	if (name)
	{
		ft_putstr_fd(name, 2);
		ft_putstr_fd(": ", 2);
	}
	if (type == ERR_EXEC)
	{
		if (err == EACCES || err == EISDIR || err == ENOEXEC)
			cmd->exit_status = 126;
		else
			cmd->exit_status = 127;
	}
	if (type == ERR_REDIR)
		cmd->exit_status = 1;
	ft_putendl_fd(strerror(err), 2);
	return (cmd->exit_status);
}

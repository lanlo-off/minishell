/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 19:15:56 by llechert          #+#    #+#             */
/*   Updated: 2025/12/16 17:33:11 by llechert         ###   ########.fr       */
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
	if (!cmd->av || !cmd->av[0])
		return (false);
	if (!cmd->path)
	{
		print_error(cmd->av[0], 0, ERR_CMD_NOT_FOUND, cmd);
		return (false);
	}
	return (true);
}

void	close_fds(int fd_in, int fd_out)
{
	if (fd_in >= 0 && !is_std_fd(fd_in))
		close(fd_in);
	if (fd_out >= 0 && !is_std_fd(fd_out))
		close(fd_out);
}

void	close_fds_ptr(int *fd_in, int *fd_out)
{
	if (fd_in && *fd_in >= 0 && !is_std_fd(*fd_in))
	{
		close(*fd_in);
		*fd_in = -1;
	}
	if (fd_out && *fd_out >= 0 && !is_std_fd(*fd_out))
	{
		close(*fd_out);
		*fd_out = -1;
	}
}

bool	is_builtin(t_cmd *cmd)
{
	if (!cmd)
		return (false);
	if (ft_strncmp(cmd->av[0], "pwd", 4) == 0)
		return (true);
	if (ft_strncmp(cmd->av[0], "echo", 5) == 0)
		return (true);
	if (ft_strncmp(cmd->av[0], "cd", 3) == 0)
		return (true);
	if (ft_strncmp(cmd->av[0], "export", 6) == 0)
		return (true);
	if (ft_strncmp(cmd->av[0], "env", 4) == 0)
		return (true);
	if (ft_strncmp(cmd->av[0], "unset", 6) == 0)
		return (true);
	if (ft_strncmp(cmd->av[0], "exit", 5) == 0)
		return (true);
	return (false);
}

int	exec_builtin(t_cmd *cmd, t_shell *shell)
{
	if (ft_strncmp(cmd->av[0], "pwd", 4) == 0)
		return (ft_pwd(cmd));
	if (ft_strncmp(cmd->av[0], "echo", 5) == 0)
		return (ft_echo(cmd->av));
	if (ft_strncmp(cmd->av[0], "cd", 3) == 0)
		return (ft_cd(cmd, shell));
	if (ft_strncmp(cmd->av[0], "env", 4) == 0)
		return (ft_env(cmd, shell));
	if (ft_strncmp(cmd->av[0], "export", 6) == 0)
		return (ft_export(cmd, shell));
	if (ft_strncmp(cmd->av[0], "unset", 6) == 0)
		return (ft_unset(cmd, shell));
	if (ft_strncmp(cmd->av[0], "exit", 5) == 0)
		return (ft_exit(cmd, shell));
	return (false);
}

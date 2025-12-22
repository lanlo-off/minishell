/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 18:26:31 by llechert          #+#    #+#             */
/*   Updated: 2025/12/22 18:26:32 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	print_error_exit(int type, char *arg)
{
	if (type == 2)
		ft_putendl_fd("exit", 2);
	ft_putstr_fd("minishell: exit: ", 2);
	if (type == 1)
	{
		ft_putstr_fd(arg, 2);
		ft_putendl_fd(": numeric argument required", 2);
	}
	if (type == 2)
		ft_putendl_fd("too many arguments", 2);
}

int	ft_exit(t_cmd *cmd, t_shell *shell)
{
	int	len_args;

	len_args = check_args(cmd->av, 2);
	if (len_args == 1)
	{
		shell->flag_exit = true;
		return (0);
	}
	if (len_args == 2 && is_numeric(cmd->av[1]))
	{
		shell->flag_exit = true;
		shell->exit_code = (unsigned int)ft_atoi(cmd->av[1]);
		return (0);
	}
	if (!is_numeric(cmd->av[1]))
	{
		shell->flag_exit = true;
		print_error_exit(1, cmd->av[1]);
		shell->exit_code = 255;
		return (0);
	}
	if (is_numeric(cmd->av[1]) && cmd->av[2])
		return (print_error_exit(2, NULL), 1);
	return (0);
}

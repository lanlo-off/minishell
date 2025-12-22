/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 18:26:27 by llechert          #+#    #+#             */
/*   Updated: 2025/12/22 18:26:28 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	ft_env(t_cmd *cmd, t_shell *shell)
{
	t_env	*temp;
	int		len_args;

	len_args = check_args(cmd->av, 1);
	temp = shell->env;
	if (len_args == 1)
	{
		while (temp)
		{
			ft_printf("%s=%s\n", temp->key, temp->value);
			temp = temp->next;
		}
	}
	else
	{
		ft_putstr_fd("env: Too many Arguments\n", 2);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

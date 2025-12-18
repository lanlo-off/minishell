/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:21:37 by llechert          #+#    #+#             */
/*   Updated: 2025/12/17 10:48:56 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	clean_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->env)
		free_env(shell->env);
	return ;
}

void	clean_exit(t_shell *shell)
{
	ft_putstr_fd("exit\n", 1);
	clean_post_parser(shell);
}

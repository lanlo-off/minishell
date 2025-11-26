/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:39:38 by llechert          #+#    #+#             */
/*   Updated: 2025/11/26 17:47:38 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// void	init_signals(shell)
// {
	
// }

void	init_structs(t_shell *shell, char **envp)
{
	if (!shell)
		return ;
	ft_bzero(shell, sizeof(t_shell));
	shell->env = NULL;
	(void) envp;
	// shell->env = get_env(envp);
	shell->exit_code = EXIT_SUCCESS;
	// init_signals(shell);//a voir avec cscarti
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:21:37 by llechert          #+#    #+#             */
/*   Updated: 2025/11/24 14:22:11 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->av)
		free(shell->av);
	if (shell->token)
		free_list(shell->token);
	free(shell);
	return ;
}

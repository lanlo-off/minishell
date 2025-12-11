/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:21:37 by llechert          #+#    #+#             */
/*   Updated: 2025/12/03 19:04:09 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	clean_shell(t_shell *shell)
{
  if (!shell)
 		return ;
  if (shell->av)
    free(shell->av);
  if (shell->env)
    free_env(shell->env);
  free(shell);
 	return ;
}

void    clean_exit(t_shell *shell)
{    
  clean_post_lexer(shell);
  clean_post_parser(shell);
  shell->exit_code = 1;
}

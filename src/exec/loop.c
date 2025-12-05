/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:05:29 by llechert          #+#    #+#             */
/*   Updated: 2025/12/05 09:25:20 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	infinite_loop(t_shell *shell)
{
	while (1)
	{
		//check le signal a intervalles frequents ?
		shell->av = readline("AU SUIVANT> ");
		if (shell->av && *shell->av)
			add_history(shell->av);
		if(!lexer(shell, shell->av))//si pb, on imprime erreur dans lexer
		{
			clean_post_lexer(shell);//on prepare la prochaine boucle en faisant free
			continue ;//et passe a la boucle suivante
		}
		if (!parser(shell, &shell->token))//on imprime l'erreur si besoin dans la fonction
		{
			clean_post_parser(shell);//inclut clean lexer dedans !
			continue ;
		}
		// if (!exec_cmd(cmd_line, env))
		// 	continue ;
		// printf("%s\n", shell->av);
		print_tokens_and_cmds(shell);
		prepare_next_loop(shell);
	}
	return (shell->exit_code);
}

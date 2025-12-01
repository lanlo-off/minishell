/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:05:29 by llechert          #+#    #+#             */
/*   Updated: 2025/12/01 16:41:55 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_tokens(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		free(token->full_token);
		// free_subwords(token->subword);
		free(token);
		token = tmp;
	}
}

int	infinite_loop(t_shell *shell)
{
	while (1)
	{
		//check le signal a intervalles frequents ?
		shell->av = readline("AU SUIVANT> ");
		if (shell->av && *shell->av)
			add_history(shell->av);
		if(!lexer(shell, shell->av))//si pb, on imprime erreur dans lexer et on set le token a NULL pour envoyer la boucle suivante
		{
			clean_lexer(shell);
			continue ;//et passe a la boucle suivante
		}
		// if (!parser(cmd_line))//on imprime l'erreur si besoin dans la fonction
		// 	continue ;
		// if (!exec_cmd(cmd_line, env))
		// 	continue ;
		// printf("%s\n", shell->av);
		print_tokens(shell->token);
		clean_lexer(shell);
		shell->token = NULL;
	}
	return (shell->exit_code);
}

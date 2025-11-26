/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:05:29 by llechert          #+#    #+#             */
/*   Updated: 2025/11/26 18:07:38 by llechert         ###   ########.fr       */
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
		// Libérer aussi les subwords si présents
		// free_subwords(token->subword);
		free(token);
		token = tmp;
	}
}

void	print_tokens(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp)
	{
		printf("full token : %s | type : %u\n", tmp->full_token, tmp->type);
		tmp = tmp->next;
	}
	return ;
}

int	infinite_loop(t_shell *shell)
{
	while (1)
	{
		//check le signal a intervalles frequents ?
		shell->av = readline("M");
		if (shell->av && *shell->av)
			add_history(shell->av);
		if(!lexer(shell, shell->av))//si pb, on imprime erreur dans lexer et on set le token a NULL pour envoyer la boucle suivante
			continue ;//et passe a la boucle suivante
		// if (!parser(cmd_line))//on imprime l'erreur si besoin dans la fonction
		// 	continue ;
		// if (!exec_cmd(cmd_line, env))
		// 	continue ;
		printf("%s\n", shell->av);
		print_tokens(shell->token);
		shell->token = NULL;
		free_tokens(shell->token);
	}
	return (shell->exit_code);
}

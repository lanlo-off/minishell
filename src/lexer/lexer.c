/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 10:43:27 by llechert          #+#    #+#             */
/*   Updated: 2025/11/24 16:53:44 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	split_token(t_shell *shell, char *line, t_lexer *lexer)
{
	int	token_len;
	
	token_len = 0;
	while (line[lexer->pos])
	{
		if (lexer->state == DEFAULT && is_operator(line[lexer->pos]))
		{
			if (token_len)
				get_token(lexer, line, shell->token);//d'abord on recupere le token juste avant l'operateur si necessaire
			handle_operator(lexer, line, shell->token);//ensuite on fait le token avec l'operateur
			token_len = 0;
		}
		else if (lexer->state == DEFAULT && line[lexer->pos] == ' ' && token_len)
		{
			get_token(lexer, line, shell->token);
			token_len = 0;
		}
		else if (//trouver la bonne condition pour incrementer)
			token_len++;//sauf quand on a un espace qui sert a rien, ou des quotes ?
		lexer->pos++;
	}
}

t_token	*lexer(t_shell *shell, char *line)
{
	t_lexer	lexer;

	if (!shell || !line || !(&lexer))
		return (NULL);
	ft_bzero(&lexer, sizeof(t_lexer));//error false, status default, pos 0
	lexer.source = line;
	if (!split_token(shell->token, lexer.source, &lexer))//split en token, voir si besoin de protection en l'ecrivant
		return (NULL);
	if (!split_subwords(shell->token))//split les mots en subword
		return (NULL);
}

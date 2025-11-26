/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:24:23 by llechert          #+#    #+#             */
/*   Updated: 2025/11/26 18:44:32 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	handle_operator(t_lexer *lexer, char *line, t_token **token_lst)
{
	t_token_type	type;

	
	if (!append_to_buffer(lexer, line[lexer->pos]))
		return (false);
	
	// Vérifier les opérateurs à 2 caractères (>> ou <<)
	if ((line[lexer->pos] == '>' || line[lexer->pos] == '<') 
		&& line[lexer->pos + 1] == line[lexer->pos])
	{
		if (!append_to_buffer(lexer, line[lexer->pos]))
			return (false);
		lexer->pos++;
	}
	
	// Déterminer le type d'opérateur
	if (lexer->buffer[0] == '|')
		type = PIPE;
	else if (ft_strlen(lexer->buffer) == 2 && lexer->buffer[0] == '<')
		type = HEREDOC;
	else if (ft_strlen(lexer->buffer) == 2 && lexer->buffer[0] == '>')
		type = APPEND;
	else if (lexer->buffer[0] == '<')
		type = REDIR_IN;
	else
		type = REDIR_OUT;
	
	// Réutiliser save_token
	if (!save_token(lexer, token_lst, type))
		return (false);
	return (true);
}
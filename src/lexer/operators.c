/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:24:23 by llechert          #+#    #+#             */
/*   Updated: 2025/11/26 15:39:24 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	handle_operator(t_lexer *lexer, char *line, t_token **token_lst)
{
	int i;

	i = lexer->pos;
	if (line[i] == '|' && !save_token(lexer, token_lst, PIPE))
		return (false);
	if (line[i] == '<')
	{
		if (line[i + 1] == line[i] && !save_token(lexer, token_lst, HEREDOC))
			return (false);
		else if (!save_token(lexer, token_lst, REDIR_IN))
			return (false);
	}
	if (line[i] == '>')
	{
		if (line[i + 1] == line[i] && save_token(lexer, token_lst, APPEND))
			return (false);
		else if (!save_token(lexer, token_lst, REDIR_OUT))
			return (false);
	}
	return (true);
}

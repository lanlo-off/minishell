/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:04:45 by llechert          #+#    #+#             */
/*   Updated: 2025/11/24 16:36:17 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_operator(char c)
{
	bool	val;
	
	val = false;
	if (c == '|' || c == '>' || c == '<')
		val = true;
	return (val);
}

bool	check_quotes(t_lexer *lexer, char c)//on checke qu'il y a une quote pour refermer
{
	int	i;

	i = lexer->pos + 1;
	while (lexer->source[i])
	{
		if (lexer->source[i] == c)
			return (true);
		i++;
	}
	return (false);
}

bool	handle_quotes(t_lexer *lexer, char *line)
{
	int	i;

	i = lexer->pos;
	if (line[i] == '\'' && lexer->state == DEFAULT)
	{
		if (!check_quotes(lexer, '\''))
			return(false);
		lexer->state = SQUOTE;
	}
	else if (line[i] == '\'' && lexer->state == SQUOTE)
		lexer->state = DEFAULT;
	else if (line[i] == '"' && lexer->state == DEFAULT)
		lexer->state = DQUOTE;
	else if (line[i] == '"' && lexer->state == DQUOTE)
		lexer->state = DEFAULT;
}
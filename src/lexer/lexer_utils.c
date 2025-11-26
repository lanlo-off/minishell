/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:04:45 by llechert          #+#    #+#             */
/*   Updated: 2025/11/26 18:02:28 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	token_add_last(t_token *new, t_token **list)
{
	t_token	*tmp;

	
	if (!list || !(*list))
	{
		*list = new;
		new->next = NULL;
		return ;
	}	
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

bool	is_operator(char c)
{
	bool	val;
	
	val = false;
	if (c == '|' || c == '>' || c == '<')
		val = true;
	return (val);
}
/**
 * @brief On checke qu'il y a le bon nombre de quotes dans les tokens
 * 
 * @param lexer 
 * @param c 
 * @return true 
 * @return false 
 */
bool	check_quotes(t_lexer *lexer, char c)
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

// bool	handle_quotes(t_lexer *lexer, char *line)
// {
// 	int	i;

// 	i = lexer->pos;
// 	if (line[i] == '\'' && lexer->state == DEFAULT)
// 	{
// 		if (!check_quotes(lexer, '\''))
// 			return(false);
// 		lexer->state = SQUOTE;
// 	}
// 	else if (line[i] == '\'' && lexer->state == SQUOTE)
// 		lexer->state = DEFAULT;
// 	else if (line[i] == '"' && lexer->state == DEFAULT)
// 		lexer->state = DQUOTE;
// 	else if (line[i] == '"' && lexer->state == DQUOTE)
// 		lexer->state = DEFAULT;
// }
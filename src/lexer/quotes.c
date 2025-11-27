/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 11:46:22 by llechert          #+#    #+#             */
/*   Updated: 2025/11/27 18:33:11 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_lex_state	initiate_state(char c)
{
	if (c == '\'')
		return (SQUOTE);
	else if (c == '"')
		return (DQUOTE);
	return (DEFAULT);
}

static bool	check_token_quotes(char *str)
{
	int			i;
	t_lex_state	state;

	if (!str)
		return (false);
	state = initiate_state(str[0]);
	i = 1;
	while (str[i])
	{
		if (state == DEFAULT && str[i] == '\'')
			state = SQUOTE;
		else if (state == DEFAULT && str[i] == '"')
			state = DQUOTE;
		else if (state == SQUOTE && str[i] == '\'')
			state = DEFAULT;
		else if (state == DQUOTE && str[i] == '"')
			state = DEFAULT;
		i++;
	}
	// printf("Je finis le check du token avec le state : %u\n", state);
	return (state == DEFAULT);
}
/**
 * @brief On checke qu'il y a le bon nombre de quotes dans les tokens
 * 
 * @param lexer 
 * @param c 
 * @return true 
 * @return false 
 */
bool	check_quotes(t_token **token_list)
{
	t_token	*tmp;

	tmp = *token_list;
	while (tmp)
	{
		// printf("je vais verifier le token : %s\n", tmp->full_token);
		if (tmp->type == WORD && !check_token_quotes(tmp->full_token))
		{
			// printf("ce token : %s n'est pas valide\n", tmp->full_token);
			return (false);
		}
		tmp = tmp->next;
	}
	// printf("tous les tokens sont valides\n");
	return (true);
}

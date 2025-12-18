/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 11:46:22 by llechert          #+#    #+#             */
/*   Updated: 2025/12/01 15:16:12 by llechert         ###   ########.fr       */
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

static void	print_error_quote(t_lex_state state)
{
	if (state == SQUOTE)
		ft_putstr_fd("error: missing single quote\n", 2);
	else
		ft_putstr_fd("error: missing double quote\n", 2);
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
	if (state != DEFAULT)
		print_error_quote(state);
	return (state == DEFAULT);
}

bool	check_quotes(t_token **token_list)
{
	t_token	*tmp;

	tmp = *token_list;
	while (tmp)
	{
		if (tmp->type == WORD && !check_token_quotes(tmp->full_token))
			return (false);
		tmp = tmp->next;
	}
	return (true);
}

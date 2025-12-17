/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_sub_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 18:17:20 by llechert          #+#    #+#             */
/*   Updated: 2025/12/17 11:31:33 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_sub_manager(t_sub_manager *man, char *str, t_token *token)
{
	man->str = str;
	man->token = token;
	man->pos = 0;
	man->sub_len = 0;
	man->state = DEFAULT;
}

bool	handle_default_sub(t_sub_manager *man)
{
	char	c;

	c = man->str[man->pos];
	if (c == '\'')
	{
		man->state = SQUOTE;
		return (create_sub(man->token, man->pos, &man->sub_len, SUB_UNQUOTED));
	}
	if (c == '"')
	{
		man->state = DQUOTE;
		return (create_sub(man->token, man->pos, &man->sub_len, SUB_UNQUOTED));
	}
	man->sub_len++;
	return (true);
}

bool	handle_squote_sub(t_sub_manager *man)
{
	char	c;

	c = man->str[man->pos];
	if (c == '\'')
	{
		man->state = DEFAULT;
		return (create_sub(man->token, man->pos, &man->sub_len, SUB_SQUOTED));
	}
	man->sub_len++;
	return (true);
}

bool	handle_dquote_sub(t_sub_manager *man)
{
	char	c;

	c = man->str[man->pos];
	if (c == '"')
	{
		man->state = DEFAULT;
		return (create_sub(man->token, man->pos, &man->sub_len, SUB_DQUOTED));
	}
	man->sub_len++;
	return (true);
}

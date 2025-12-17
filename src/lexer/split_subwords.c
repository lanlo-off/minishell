/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_subwords.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 16:01:34 by llechert          #+#    #+#             */
/*   Updated: 2025/12/17 11:31:39 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	append_subword(t_subword *new, t_subword **list)
{
	t_subword	*tmp;
	
	if (!list || !*list)
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

static bool	create_empty_sub(t_sub_type type, t_token *token)
{
	t_subword *new_sub;
	
	if (type == DEFAULT)
		return (true);
	new_sub = ft_calloc(1, sizeof(t_subword));
	if (!new_sub)
		return (false);
	new_sub->next = NULL;
	new_sub->type = type;
	new_sub->val = ft_calloc(1, 1);
	if (!new_sub->val)
		return (free(new_sub), false);
	new_sub->val[0] = 0;
	append_subword(new_sub, &token->subword);
	return (true);
}

bool	create_sub(t_token *token, int end, int *length, t_sub_type type)
{
	int			i;
	int			start;
	t_subword	*new_sub;

	if (!length || !*length)
		return (create_empty_sub(type, token));
	i = 0;
	start = end - *length;
	new_sub = malloc(sizeof(t_subword));
	if (!new_sub)
		return (false);
	new_sub->next = NULL;
	new_sub->type = type;
	new_sub->val = malloc((*length) + 1);
	if (!new_sub->val)
		return (free(new_sub), false);
	while (i < *length)
	{
		new_sub->val[i] = token->full_token[start + i];
		i++;
	}
	new_sub->val[i] = 0;
	append_subword(new_sub, &token->subword);
	*length = 0;
	return (true);
}

static bool	split_subs(char *str, t_token *token)
{
	t_sub_manager	man;
	
	if (!str || !token)
		return (true);
	set_sub_manager(&man, str, token);
	while (man.str[man.pos])
	{
		if (man.state == DEFAULT)
		{
			if (!handle_default_sub(&man))
				return (false);
		}
		else if (man.state == SQUOTE)
		{
			if (!handle_squote_sub(&man))
				return (false);
		}
		else if (man.state == DQUOTE)
		{
			if (!handle_dquote_sub(&man))
				return (false);
		}
		man.pos++;
	}
	return (create_sub(token, man.pos, &man.sub_len, SUB_UNQUOTED));//gerer le dernier mot si jamais il se finit pas par des quotes (sinon c'est gere dans la boucle)
}

bool	split_subwords(t_token **token_list)
{
	t_token	*tmp;

	tmp = *token_list;
	while (tmp)
	{
		if (!split_subs(tmp->full_token, tmp))
			return (false);
		tmp = tmp->next;
	}
	return (true);
}

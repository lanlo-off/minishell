/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 18:28:45 by llechert          #+#    #+#             */
/*   Updated: 2025/12/07 13:56:02 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	token_add_last(t_token *new, t_token **list)
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

static bool	create_buffer_append(t_lexer *lexer, char c)
{
	lexer->buffer = malloc(2);
	if (!lexer->buffer)
		return (false);
	lexer->buffer[0] = c;
	lexer->buffer[1] = 0;
	lexer->buffer_size = 2;
	return (true);
}

bool	append_to_buffer(t_lexer *lexer, char c)
{
	char	*new_buffer;

	if (!lexer->buffer)
		return (create_buffer_append(lexer, c));
	if ((int)ft_strlen(lexer->buffer) + 1 >= lexer->buffer_size)
	{
		lexer->buffer_size *= 2;
		new_buffer = malloc(lexer->buffer_size);
		if (!new_buffer)
			return (false);
		ft_bzero(new_buffer, lexer->buffer_size);
		ft_strcpy(new_buffer, lexer->buffer);
		free(lexer->buffer);
		lexer->buffer = new_buffer;
	}
	lexer->buffer[ft_strlen(lexer->buffer)] = c;
	lexer->buffer[ft_strlen(lexer->buffer)] = 0;
	return (true);
}

bool	save_token(t_lexer *lexer, t_token **token, t_token_type type)
{
	t_token	*new;

	if (!lexer->buffer || lexer->buffer[0] == 0)
		return (true);
	new = malloc(sizeof(t_token));
	if (!new)
		return (false);
	new->type = type;
	new->full_token = ft_strdup(lexer->buffer);
	new->subword = NULL;
	new->next = NULL;
	token_add_last(new, token);
	free(lexer->buffer);
	lexer->buffer = NULL;
	lexer->buffer_size = 0;
	return (true);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 15:18:03 by llechert          #+#    #+#             */
/*   Updated: 2025/12/05 10:10:03 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	clean_subword(t_subword **subword_list)
{
	t_subword	*tmp;
	t_subword	*to_free;

	if (!subword_list || !*subword_list)
		return ;
	tmp = *subword_list;
	while (tmp)
	{
		to_free = tmp;
		tmp = tmp->next;
		free(to_free->val);
		free(to_free);
	}
}

static void	clean_token(t_token **token_list)
{
	t_token	*tmp;
	t_token	*to_free;

	if (!token_list || !*token_list)
		return ;
	tmp = *token_list;
	while (tmp)
	{
		to_free = tmp;
		tmp = tmp->next;
		clean_subword(&to_free->subword);
		free(to_free);
	}
}

void	clean_post_lexer(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->av)
		free(shell->av);
	if (shell->token)
		clean_token(&shell->token);
	shell->token = NULL;
}

void	clean_lexer_struct(t_lexer *lexer)
{
	if (!lexer)
		return ;
	if (lexer->buffer)
		free(lexer->buffer);
}

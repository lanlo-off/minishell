/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:24:23 by llechert          #+#    #+#             */
/*   Updated: 2025/11/24 16:03:30 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_operator(t_lexer *lexer, char *line, t_token *token_lst)
{
	int i;

	i = lexer->pos;
	if (line[i] == '|')
		create_token(1, NULL, token_lst);
	if (line[i] == '<')
	{
		if (line[i + 1] == line[i])
			create_token(2, NULL, token_lst);
		else
			create_token(3, NULL, token_lst);
	}
	if (line[i] == '>')
	{
		if (line[i + 1] == line[i])
			create_token(4, NULL, token_lst);
		else
			create_token(5, NULL, token_lst);
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 10:43:27 by llechert          #+#    #+#             */
/*   Updated: 2025/12/16 17:06:39 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	handle_default_tok(t_shell *shell, char *line, t_lexer *lexer)
{
	char	c;

	c = line[lexer->pos];
	if (is_operator(c))
	{
		if (!save_token(lexer, &shell->token, WORD))
			return (false);
		if (!handle_operator(lexer, line, &shell->token))
			return (false);
		return (true);
	}
	else if (c == ' ')
		return (save_token(lexer, &shell->token, WORD));
	else if (c == '\'')
		lexer->state = SQUOTE;
	else if (c == '"')
		lexer->state = DQUOTE;
	return (append_to_buffer(lexer, c));
}

static bool	handle_squote_tok(char *line, t_lexer *lexer)
{
	if (line[lexer->pos] == '\'')
		lexer->state = DEFAULT;
	return (append_to_buffer(lexer, line[lexer->pos]));
}

static bool	handle_dquote_tok(char *line, t_lexer *lexer)
{
	if (line[lexer->pos] == '"')
		lexer->state = DEFAULT;
	return (append_to_buffer(lexer, line[lexer->pos]));
}

static bool	split_token(t_shell *shell, char *line, t_lexer *lexer)
{
	while (line[lexer->pos])
	{
		if (lexer->state == DEFAULT)
		{
			if (!handle_default_tok(shell, line, lexer))
				return (false);
		}
		else if (lexer->state == SQUOTE)
		{
			if (!handle_squote_tok(line, lexer))
				return (false);
		}
		else if (lexer->state == DQUOTE)
		{
			if (!handle_dquote_tok(line, lexer))
				return (false);
		}
		lexer->pos++;
	}
	if (!save_token(lexer, &shell->token, WORD))
		return (false);
	return (true);
}

bool	lexer(t_shell *shell, char *line)
{
	t_lexer	lexer;

	if (!shell || !line)
		return (NULL);
	ft_bzero(&lexer, sizeof(t_lexer));
	lexer.source = line;
	if (!split_token(shell, lexer.source, &lexer))
		return (clean_lexer_struct(&lexer), false);
	if (!check_quotes(&shell->token))
	{
		shell->exit_code = EXIT_SYNTAX_ERROR;
		clean_lexer_struct(&lexer);
		return (false);
	}
	if (!split_subwords(&shell->token))
		return (clean_lexer_struct(&lexer), false);
	clean_lexer_struct(&lexer);
	return (true);
}

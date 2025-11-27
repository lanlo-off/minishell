/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 10:43:27 by llechert          #+#    #+#             */
/*   Updated: 2025/11/27 18:38:11 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	handle_default_tok(t_shell *shell, char *line, t_lexer *lexer)
{
	char	c;

	c = line[lexer->pos];
	if (is_operator(c))
	{
		if (!save_token(lexer, &shell->token, WORD))//d'abord on recupere le token juste avant l'operateur (si vide rien ne se cree)
			return (false);
		if (!handle_operator(lexer, line, &shell->token))//ensuite on fait le token avec l'operateur
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
/**
 * @brief Separe l'input en tokens et gere le statut du lexer
 * Separe en fonction des operateurs et espaces en dehors des quotes
 * Appelle Save_token meme si le buffer est vide car save token est protege
 * et ne fait rien si buffer vide
 * 
 * @param shell 
 * @param line 
 * @param lexer 
 * @return true 
 * @return false 
 */
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
	if (!save_token(lexer, &shell->token, WORD))//pour le dernier token s'il se termine par le /0, sinon il est traite dans la boucle et s'il est vide (fini par un espace ou un operateur) ca ne cree pas de token
		return (false);
	return (true);
}

/**
 * @brief separe en tokens
 * Puis checke le nombre de quotes "qui comptent" pour s'assurer que tout se referme correctement
 * Puis separe les tokens en subwords si necessaire
 * 
 * @param shell 
 * @param line l'input 
 * @return t_token* la liste chainee de tokens ou NULL si erreur quelque part
 * ou si pb dans les quotes 
 */
bool	lexer(t_shell *shell, char *line)
{
	t_lexer	lexer;

	if (!shell || !line)
		return (NULL);
	ft_bzero(&lexer, sizeof(t_lexer));//error false, status default, pos 0
	lexer.source = line;
	if (!split_token(shell, lexer.source, &lexer))//split en token, voir si besoin de protection en l'ecrivant
		return (false);
	if (!check_quotes(&shell->token))//verifie que chaque quote a bien sa complementaire pour se refermer (sauf les quotes dans les quotes)
		return (false);
	if (!split_subwords(&shell->token))//split les mots en subword
		return (false);
	return (true);
}

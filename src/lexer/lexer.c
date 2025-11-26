/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 10:43:27 by llechert          #+#    #+#             */
/*   Updated: 2025/11/26 17:50:25 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Ajoute le char envoye au buffer du lexer
 * Checke si le buffer existe ou pas
 * Si plus de place dans le buffer, on l'agrandit en faisant size *2
 * 
 * @param lexer 
 * @param c char envoye
 * @return true tout s'est bien passe
 * @return false pb de malloc
 */
bool	append_to_buffer(t_lexer *lexer, char c)
{
	char	*new_buffer;
	
	if (!lexer->buffer)//si pas encore de buffer cree
	{
		lexer->buffer = malloc(2);
		if (!lexer->buffer)
			return (false);
		lexer->buffer[0] = c;
		lexer->buffer[0] = 0;
		lexer->buffer_size = 2;
		return (true);
	}
	if ((int)ft_strlen(lexer->buffer) > lexer->buffer_size)//si plus de place dans le buffer
	{
		lexer->buffer_size *= 2;
		new_buffer = malloc(lexer->buffer_size);
		if (!new_buffer)
			return (false);
		ft_strcpy(lexer->buffer, new_buffer);//a faire proprement
		free(lexer->buffer);
		lexer->buffer = new_buffer;
	}
	lexer->buffer[ft_strlen(lexer->buffer)] = c;//on append le charactere
	lexer->buffer[ft_strlen(lexer->buffer) + 1] = 0;//on termine pour pouvoir manipuler correctement le token plus tard
	return (true);
}

/**
 * @brief Verifie si buffer existe ou n'est pas vide (evite de tokeniser les espaces)
 * Puis duplique le buffer du lexer pour faire un vrai token et l'ajouter a la liste
 * Cleane le lexer pour preparer le prochain token
 * 
 * @param lexer 
 * @param token envoyer la tete de liste
 * @param type de token qu'on veut creer
 * @return true si creation de token ou si rien a cree
 * @return false si pb de malloc
 */
bool	save_token(t_lexer *lexer, t_token **token, t_token_type type)
{
	t_token	*new;

	if (!lexer->buffer || lexer->buffer[0] == 0)//si pas de buffer ou si buffer vide
		return (true);//on renvoie vrai mais on cree rien (par ex si 2 espaces de suite dans l'input)
	new = malloc(sizeof(t_token));
	if (!new)
		return (false);
	new->type = type;
	new->full_token = ft_strdup(lexer->buffer);
	new->subword = NULL;
	new->next = NULL;
	token_add_last(new, token);
	free(lexer->buffer);//on cleane le lexer pour passer au prochain token
	lexer->buffer = NULL;
	lexer->buffer_size = 0;
	return (true);
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
bool	split_token(t_shell *shell, char *line, t_lexer *lexer)
{
	while (line[lexer->pos])
	{
		if (lexer->state == DEFAULT && is_operator(line[lexer->pos]))
		{
			if (!save_token(lexer, &shell->token, WORD))//d'abord on recupere le token juste avant l'operateur (si vide rien ne se cree)
				return (false);
			if (!handle_operator(lexer, line, &shell->token))//ensuite on fait le token avec l'operateur
				return (false);
		}
		else if (lexer->state == DEFAULT && line[lexer->pos] == ' ')
		{
			if (!save_token(lexer, &shell->token, WORD))
				return (false);
		}
		else if (lexer->state == DEFAULT && line[lexer->pos] == '\'')
		{
			lexer->state = SQUOTE;
			if (!append_to_buffer(lexer, line[lexer->pos]))
				return (false);
		}
		else if (lexer->state == DEFAULT && line[lexer->pos] == '"')
		{
			lexer->state = DQUOTE;
			if (!append_to_buffer(lexer, line[lexer->pos]))
				return (false);
		}
		else if (lexer->state == SQUOTE && line[lexer->pos] == '\'')
		{
			lexer->state = DEFAULT;
			if (!append_to_buffer(lexer, line[lexer->pos]))
				return (false);
		}
		else if (lexer->state == DQUOTE && line[lexer->pos] == '"')
		{
			lexer->state = DEFAULT;
			if (!append_to_buffer(lexer, line[lexer->pos]))
				return (false);
		}
		else if (line[lexer->pos] != ' ')
		{
			if (!append_to_buffer(lexer, line[lexer->pos]))
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
	// if (!check_quotes(shell->token))//verifie que chaque quote a bien sa complementaire pour se refermer (sauf les quotes dans les quotes)
	// 	return (false);
	// if (!split_subwords(shell->token))//split les mots en subword
	// 	return (false);
	return (true);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 18:28:45 by llechert          #+#    #+#             */
/*   Updated: 2025/11/27 18:42:12 by llechert         ###   ########.fr       */
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
		return (create_buffer_append(lexer, c));
	if ((int)ft_strlen(lexer->buffer) + 1 >= lexer->buffer_size)//si plus de place dans le buffer
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
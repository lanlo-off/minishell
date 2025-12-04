/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 15:00:42 by llechert          #+#    #+#             */
/*   Updated: 2025/12/04 11:56:32 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Get the var end pos index
 * Start on the char after $ not on the $
 * 
 * @param str 
 * @param start first char after $ (not $)
 * @return int index of the last char of the var
 */
int	get_var_end_pos(char *str, int start)
{
	int	end;

	end = start;
	if (!str[end])//pas de caracteres apres le $
		return (end - 1);
	if (str[end] == '?' || ft_isdigit(str[end]) || str[end] == '$')//si ? c'est la variable particuliere, si numerique ca consomme le premier nombre
		return (end);
	if (!ft_isalpha(str[end]) || str[end] != '_')//dans ce cas on ne consomme que le $ (car le cas du $0-9 est deja gere au dessus)
		return (end - 1);
	end++;//premier caractere est forcement alphabetique ou _
	while (str[end] && (ft_isalnum(str[end]) || str[end] == '_'))//les suivants sont alphanum ou _
		end++;
	return (end - 1);//si le premier caractere est invalide, on renvoie donc l'index de ce dernier comme fin de variable, fonctionne aussi pour $?
}

char	*expanded(char *str, t_sub_type type, t_shell *shell)
{
	int		i;
	char	*res;
	char	*chunk;
	
	if (type == SUB_SQUOTED)
		return (ft_strdup(str));
	i = 0;
	res = ft_strdup("");
	while (str[i])
	{
		res = append_until_doll(str, &i, res);
		if (!str[i])
			break ;//on est a la fin de la string on n'a pas rencontre de dollar
		i++;//sinon on skip le $
		chunk = expand_var(str, &i, shell);
		res = join_and_free(res, chunk);
	}
	return (res);
}

// int	count_size_needed(t_subword *subword)
// {
// 	int			count;
// 	t_subword	*tmp;
// 	int			i;

// 	count = 0;
// 	tmp = subword;
// 	while (tmp)
// 	{
// 		i = 0;
// 		while (tmp->val[i] && tmp->val[i] != '$')
// 		{
// 			count++;
// 			i++;
// 		}
// 		if (tmp->val[i] && tmp->val[i] == '$')

		
// 		tmp = tmp->next;
// 	}
// }

// char	*expanded(char *str, t_shell *shell)
// {
// 	int		i;
// 	int		end;
// 	char	*res;
// 	char	*tmp;
// 	char	*var;
	
// 	i = 0;
// 	res = ft_calloc(1, ft_strlen(str));//on demarre avec ca et quand on tombe sur une variable on fait la modif
// 	if (!res)
// 		return (NULL);
// 	while (str[i])
// 	{
// 		tmp = res;
// 		while (str[i] && str[i] != '$')//copie tant qu'on rencontre pas un $
// 		{
// 			res[i] = str[i];
// 			i++;
// 		}
// 		tmp = res;
// 		if (str[i] == '$')
// 			end = get_end_var(str, i);
// 		var = ft_strndup(&str[i + 1], end - i);
// 		res = 
// 	}
// }
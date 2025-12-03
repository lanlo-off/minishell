/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 15:00:42 by llechert          #+#    #+#             */
/*   Updated: 2025/12/03 19:08:14 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_expanded_var(char *var, t_env **env)
{
	char	*res;
	int		i;
	t_env	*tmp;

	i = 0;
	if (!env || !*env)
		return (NULL);
	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(var, tmp->key))//si on trouve bien la key
		{
			res = ft_strdup(tmp->value);
			if (!res)
				return (NULL);
			return (res);
		}
		tmp=tmp->next;
	}
	return (NULL);
}

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

static bool	is_variable_valid(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (true);
	return (false);
}

static int	get_var_end_pos(char *str, int start)//je start pas sur le $ mais sur le caractere d'apres
{
	int	end;

	end = start;
	// if (str[end] == '?')//si ? c'est la variable particuliere
	// 	return (end);
	if (ft_isalpha(str[end]) || str[end] == '_')//premier caractere est forcement alphabetique ou _
	{
		while (str[end] && is_variable_valid(str[end]))
			end++;
		return (end - 1);
	}
	return (end);//si le premier caractere est invalide, on renvoie donc l'index de ce dernier comme fin de variable
}

char	*expanded(char *str, t_sub_type type, t_shell *shell)
{
	int		start;
	int		end;
	char	*res;
	char	*tmp;
	char	*var;
	
	if (type == SUB_SQUOTED)
		return (ft_strdup(str));
	start = 0;
	while (str[start])
	{
		end = start;
		while (str[start] && str[start] != '$')
			start++;
		tmp = ft_strndup(&str[end], start);
		if (str[start++] == '$')//j'avance pour passer le $
			end = get_var_end_pos(str, start);
		var = ft_strndup(&str[start], end - start + 1);
		res = ft_strjoin(tmp, get_expanded_var(var, &shell->env));
		free(tmp);
		free(var);
		start = end + 1;
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

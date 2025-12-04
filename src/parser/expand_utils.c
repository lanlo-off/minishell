/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 11:29:36 by llechert          #+#    #+#             */
/*   Updated: 2025/12/04 12:07:36 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_expanded_var(char *var, t_env **env, t_shell *shell)
{
	t_env	*tmp;

	if (!env || !*env || !var)//si rien a trouver on renvoie vide
		return (ft_strdup(""));
	if (!ft_strcmp(var, "$") || !ft_strcmp(var, "#"))//cas particuliers qu'on veut ignorer
		return (ft_strdup(""));
	if (!ft_strcmp(var, "$?"))//cas particulier dernier exit code
		return ((void) shell, "$? = exit code precedent");//il faut renvoyer l'exit code precedent
	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(var, tmp->key))//si on trouve bien la key
		{
			if (tmp->value)//s'il y a une value associee
				return (ft_strdup(tmp->value));
			return (ft_strdup(""));
		}
		tmp=tmp->next;
	}
	return (ft_strdup(""));//variable pas trouvee
}

char	*join_and_free(char *s1, char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}
/**
 * @brief 
 * 
 * @param str 
 * @param start index du premier caractere a copier
 * @param end index du premier caractere a ne pas copier
 * @return char* 
 */
static char	*copy_literal(char *str, int start, int end)
{
	return (ft_strndup(str + start, end - start));//est on sur du calcul ici ?
}

char	*expand_var(char *str, int *i, t_shell *shell)
{
	int		start;
	int		end;
	char	*varname;
	char	*value;

	start = *i;
	end = get_var_end_pos(str, start);
	if (end < start)
		return (ft_strdup("$"));
	varname = ft_strndup(str + start, end - start + 1);
	value = get_expanded_var(varname, &shell->env, shell);
	free(varname);
	*i = end + 1;//on avance l'index pour pas reboucler sur les caracteres de la variable dans la copie until litteral
	return (value);
}

char	*append_until_doll(char *str, int *i, char *res)
{
	int		start;
	char	*lit;

	start = *i;
	while (str[*i] && str[*i] != '$')
		(*i)++;
	lit = copy_literal(str, start, *i);
	return (join_and_free(res, lit));
}

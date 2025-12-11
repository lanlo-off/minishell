/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 11:29:36 by llechert          #+#    #+#             */
/*   Updated: 2025/12/05 15:56:36 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	get_next_useful_token(t_token **token, t_token_type type)
{
	*token = (*token)->next;//on avance car on vient de traiter le token envoye
	if (type != WORD && type != PIPE)//si c'etait une redir avant on est sur qu'il y aura un token apres
		*token = (*token)->next;//on skip le token WORD = file du redir car deja traite avec le redir
}

char	*get_expanded_var(char *var, t_env **env, t_shell *shell)
{
	t_env	*tmp;

	if (!env || !*env || !var)//si rien a trouver on renvoie vide
		return (ft_strdup(""));
	if (!ft_strcmp(var, "$") || !ft_strcmp(var, "#"))//cas particuliers qu'on veut ignorer
		return (ft_strdup(""));
	if (!ft_strcmp(var, "$?"))//cas particulier dernier exit code
		return (ft_itoa(shell->exit_code));//A bien garder entre chaque boucle voir comment gerer si on commence par faire $? dans minishell
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

/**
 * @brief Get the var end pos index
 * Start on the char after $ not on the $
 * 
 * @param str 
 * @param start first char after $ (not $)
 * @return int index of the last char of the var
 */
static int	get_var_end_pos(char *str, int start)
{
	int	end;

	end = start;
	if (!str[end])//pas de caracteres apres le $
		return (end - 1);
	if (str[end] == '?' || ft_isdigit(str[end]) || str[end] == '$')//si ? c'est la variable particuliere, si numerique ca consomme le premier nombre
		return (end);
	if (!ft_isalpha(str[end]) && str[end] != '_')//dans ce cas on ne consomme que le $ (car le cas du $0-9 est deja gere au dessus)
		return (end - 1);
	end++;//premier caractere est forcement alphabetique ou _
	while (str[end] && (ft_isalnum(str[end]) || str[end] == '_'))//les suivants sont alphanum ou _
		end++;
	return (end - 1);//si le premier caractere est invalide, on renvoie donc l'index de ce dernier comme fin de variable, fonctionne aussi pour $?
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
		return (ft_strdup("$"));//Pourquoi j'ai mis ca deja ??
	varname = ft_strndup(str + start, end - start + 1);
	value = get_expanded_var(varname, &shell->env, shell);
	free(varname);
	*i = end + 1;//on avance l'index pour pas reboucler sur les caracteres de la variable dans la copie until litteral
	return (value);
}

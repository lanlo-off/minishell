/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 11:29:36 by llechert          #+#    #+#             */
/*   Updated: 2025/12/17 15:05:43 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	has_quoted_subword(t_subword *sub)
{
	t_subword	*tmp;

	if (!sub)
		return (false);
	tmp = sub;
	while (tmp)
	{
		if (tmp->type == SUB_SQUOTED || tmp->type == SUB_DQUOTED)
			return (true);
		tmp = tmp->next;
	}
	return (false);
}

void	get_next_useful_token(t_token **token, t_token_type type)
{
	*token = (*token)->next;
	if (type != WORD && type != PIPE)
		*token = (*token)->next;
}

char	*get_expanded_var(char *var, t_env **env, t_shell *shell)
{
	t_env	*tmp;

	if (!env || !*env || !var)
		return (ft_strdup(""));
	if (!ft_strcmp(var, "$") || !ft_strcmp(var, "#"))
		return (ft_strdup(""));
	if (!ft_strcmp(var, "?"))
		return (ft_itoa(shell->exit_code));
	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(var, tmp->key))
		{
			if (tmp->value)
				return (ft_strdup(tmp->value));
			return (ft_strdup(""));
		}
		tmp = tmp->next;
	}
	return (ft_strdup(""));
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
	if (!str[end])
		return (end - 1);
	if (str[end] == '?' || ft_isdigit(str[end]) || str[end] == '$')
		return (end);
	if (!ft_isalpha(str[end]) && str[end] != '_')
		return (end - 1);
	end++;
	while (str[end] && (ft_isalnum(str[end]) || str[end] == '_'))
		end++;
	return (end - 1);
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
	*i = end + 1;
	return (value);
}

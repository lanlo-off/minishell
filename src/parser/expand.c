/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 15:00:42 by llechert          #+#    #+#             */
/*   Updated: 2025/12/11 14:59:42 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*join_and_free(char *s1, char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}

char	*append_until_doll(char *str, int *i, char *res)
{
	int		start;
	char	*lit;

	start = *i;
	while (str[*i] && str[*i] != '$')
		(*i)++;
	lit = ft_strndup(str + start, *i - start);
	return (join_and_free(res, lit));
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
			break ;
		i++;
		chunk = expand_var(str, &i, shell);
		res = join_and_free(res, chunk);
	}
	return (res);
}

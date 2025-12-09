/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 19:06:01 by llechert          #+#    #+#             */
/*   Updated: 2025/12/09 19:21:07 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**split_path(t_env *env)
{
	t_env	*path_var;
	char	**paths;

	if (!env)
		return (NULL);
	path_var = env;
	while (path_var)
	{
		if (ft_strncmp(path_var->key, "PATH", 5) == 0)
		{
			paths = ft_split(path_var->value, ':');
			return (paths);
		}
		path_var = path_var->next;
	}
	return (NULL);
}

char	*get_path(char *cmd, t_env	*env)
{
	int		i;
	char	**all_path;
	char	*tmp_path;
	char	*full_path;

	all_path = split_path(env);
	if (!all_path)
		return (NULL);
	i = 0;
	while (all_path[i])
	{
		tmp_path = ft_strjoin(all_path[i], "/");
		full_path = ft_strjoin(tmp_path, cmd);
		free(tmp_path);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			free_tab_str(all_path);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_tab_str(all_path);
	return (NULL);
}
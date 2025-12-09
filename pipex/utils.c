/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:46:32 by llechert          #+#    #+#             */
/*   Updated: 2025/07/07 16:59:38 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

char	**split_path(char **envp)
{
	int		i;
	int		j;
	char	*sub;
	char	**paths;

	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && envp[i][j] != '=')
			j++;
		sub = ft_substr(envp[i], 0, j);
		if (!sub)
			return (NULL);
		if (ft_strncmp(sub, "PATH", 5) == 0)
		{
			free(sub);
			paths = ft_split(envp[i] + j + 1, ':');
			return (paths);
		}
		free(sub);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **envp)
{
	int		i;
	char	**all_path;
	char	*tmp_path;
	char	*full_path;

	all_path = split_path(envp);
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

void	dup2_fds(int fd_in, int fd_out)
{
	dup2(fd_in, STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
}

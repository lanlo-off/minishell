/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:30:44 by llechert          #+#    #+#             */
/*   Updated: 2025/07/07 17:51:01 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	free_path(char *path, int path_to_free)
{
	if (path_to_free)
		free(path);
}

void	exit_bad_fork(t_cmd *tab_cmds, t_args *args)
{
	ft_putstr_fd("Could not fork!\n", 2);
	exit_error(tab_cmds, args, args->nb_cmd, 1);
}

void	free_cmds(t_cmd *tab_cmds, int tab_size)
{
	int	i;

	i = 0;
	while (i < tab_size)
	{
		free_path(tab_cmds[i].path, tab_cmds[i].path_to_free);
		if (tab_cmds[i].cmd_split)
			free_tab_str(tab_cmds[i].cmd_split);
		i++;
	}
	free(tab_cmds);
}

void	exit_error(t_cmd *tab_cmds, t_args *args, int tab_size, int error)
{
	free_cmds(tab_cmds, tab_size);
	free(args);
	exit(error);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 09:23:44 by llechert          #+#    #+#             */
/*   Updated: 2025/12/09 19:00:23 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_args	*args;
	t_cmd	*tab_cmds;
	int		status;

	if (argc < 5 || (ft_strcmp(argv[1], "here_doc") == 0 && argc < 6))
		return (ft_putstr_fd("Not enough arguments!\n", 1), 1);
	args = ft_calloc(1, sizeof(t_args));
	if (!args)
		return (1);
	fill_args(args, argc, argv, envp);
	tab_cmds = init_cmds(args);
	if (!tab_cmds)
		return (1);
	pipex(tab_cmds, args);
	close_fds(tab_cmds[0].fd_in, tab_cmds[args->nb_cmd - 1].fd_out);
	status = wait_children(tab_cmds, args->nb_cmd);
	free_cmds(tab_cmds, args->nb_cmd);
	free(args);
	return (status);
}

void	fill_args(t_args *args, int argc, char **argv, char **envp)
{
	args->ac = argc;
	args->hd = (ft_strcmp(argv[1], "here_doc") == 0);
	if (args->hd)
		args->limiter = argv[2];
	args->nb_cmd = argc - 3 - args->hd;
	args->av = argv;
	args->env = envp;
}

t_cmd	*init_cmds(t_args *args)
{
	t_cmd	*tab_cmds;
	int		i;

	i = 0;
	tab_cmds = ft_calloc(args->nb_cmd, sizeof(t_cmd));
	if (!tab_cmds)
		return (NULL);
	while (i < args->nb_cmd)
		fill_cmd(tab_cmds, args, i++);
	return (tab_cmds);
}

void	fill_cmd(t_cmd *tab_cmds, t_args *args, int i)
{
	tab_cmds[i].cmd_nb = i;
	tab_cmds[i].cmd = args->av[i + 2 + args->hd];
	tab_cmds[i].cmd_split = ft_split(tab_cmds[i].cmd, ' ');
	tab_cmds[i].path_to_free = 0;
	if (tab_cmds[i].cmd_split && tab_cmds[i].cmd_split[0])
	{
		if (ft_strchr(tab_cmds[i].cmd_split[0], '/') == NULL)
		{
			tab_cmds[i].path = get_path(tab_cmds[i].cmd_split[0], args->env);
			tab_cmds[i].path_to_free = 1;
		}
		else
			tab_cmds[i].path = tab_cmds[i].cmd_split[0];
	}
	tab_cmds[i].fd_in = -1;
	tab_cmds[i].fd_out = -1;
	if (!i && !args->hd)
		tab_cmds[i].fd_in = open_infile(args->av[1]);
	else if (i == args->nb_cmd - 1)
		tab_cmds[i].fd_out = open_outfile(args->av[args->ac - 1], args->hd);
}

int	wait_children(t_cmd *tab_cmds, int nb_cmd)
{
	int	i;
	int	code;

	i = 0;
	while (i < nb_cmd)
	{
		waitpid(tab_cmds[i].pid, &tab_cmds[i].exit_status, 0);
		i++;
	}
	code = tab_cmds[i - 1].exit_status;
	if (WIFEXITED(code))
		return (WEXITSTATUS(code));
	else if (WIFSIGNALED(code))
		return (128 + WTERMSIG(code));
	else
		return (1);
}

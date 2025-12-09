/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:57:40 by llechert          #+#    #+#             */
/*   Updated: 2025/07/07 18:53:39 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	read_line(int *pipefd, char *limiter)
{
	char	*line;
	int		limiter_reached;

	limiter_reached = 0;
	write(1, "heredoc> ", 9);
	line = get_next_line(0);
	while (line)
	{
		write(1, "heredoc> ", 9);
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
		{
			limiter_reached = 1;
			break ;
		}
		ft_putstr_fd(line, pipefd[1]);
		free(line);
		line = get_next_line(0);
	}
	if (!limiter_reached)
		ft_putendl_fd("warning: there is no limiter", 2);
	if (line)
		free(line);
}

void	handle_here_doc(t_cmd *tab_cmds, t_args *args)
{
	int		pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe here_doc");
		exit (1);
	}
	read_line(pipefd, args->limiter);
	if (pipefd[1] >= 0)
		close(pipefd[1]);
	tab_cmds->fd_in = pipefd[0];
}

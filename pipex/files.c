/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:39:09 by llechert          #+#    #+#             */
/*   Updated: 2025/12/09 19:00:28 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	open_infile(char *file)
{
	int	fd;

	fd = -1;
	fd = open(file, O_RDONLY);
	if (fd < 0)
		ft_putstr_fd("Could not read infile!\n", 2);
	return (fd);
}

int	open_outfile(char *file, int here_doc)
{
	int	fd;

	fd = -1;
	if (here_doc == 0)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fd < 0)
		ft_putstr_fd("Could not open outfile!\n", 2);
	return (fd);
}

void	close_fds(int fd_in, int fd_out)
{
	if (fd_in >= 0 && fd_in != STDIN_FILENO && fd_in != STDOUT_FILENO
		&& fd_in != STDERR_FILENO)
		close(fd_in);
	if (fd_out >= 0 && fd_out != STDIN_FILENO && fd_out != STDOUT_FILENO
		&& fd_out != STDERR_FILENO)
		close(fd_out);
}

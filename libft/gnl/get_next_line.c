/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:12:39 by llechert          #+#    #+#             */
/*   Updated: 2025/06/18 14:23:08 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static void	fill_list(t_list **list, int fd)
{
	int		nb_read;
	char	*buf;

	while (!find_newline(*list))
	{
		buf = malloc(BUFFER_SIZE + 1);
		if (!buf)
			return ;
		nb_read = read(fd, buf, BUFFER_SIZE);
		if (!nb_read)
		{
			free(buf);
			return ;
		}
		buf[nb_read] = 0;
		add_new_element(list, buf);
	}
}

static char	*get_line(t_list *list)
{
	char	*next_line;
	int		i;
	int		k;

	next_line = (char *)malloc(ft_len_needed(&list) + 1);
	i = 0;
	while (list)
	{
		k = 0;
		while (((char *)list->content)[k] && ((char *)list->content)[k] != '\n')
		{
			next_line[i + k] = ((char *)list->content)[k];
			k++;
		}
		if (((char *)list->content)[k] == '\n')
		{
			next_line[i + k] = '\n';
			k++;
		}
		i += k;
		list = list->next;
	}
	next_line[i] = 0;
	return (next_line);
}

static void	prepare_next(t_list **list)
{
	int		i;
	char	*rest;
	t_list	*last;

	i = 0;
	rest = NULL;
	last = ft_lstlast(*list);
	while (((char *)last->content)[i] && ((char *)last->content)[i] != '\n')
		i++;
	if (((char *)last->content)[i] == '\n' && ((char *)last->content)[i + 1])
	{
		i++;
		rest = ft_strdup(last->content + i);
	}
	prepare_list(list, rest);
}

char	*get_next_line(int fd)
{
	char			*next_line;
	static t_list	*list;

	next_line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		prepare_list(&list, NULL);
		return (NULL);
	}
	fill_list(&list, fd);
	if (!list)
		return (NULL);
	next_line = get_line(list);
	prepare_next(&list);
	return (next_line);
}

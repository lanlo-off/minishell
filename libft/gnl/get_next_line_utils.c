/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:12:44 by llechert          #+#    #+#             */
/*   Updated: 2025/06/18 14:21:55 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	add_new_element(t_list	**list, char *buf)
{
	t_list	*new;
	t_list	*temp;

	new = malloc(sizeof(t_list));
	if (!new)
	{
		return ;
	}
	new->content = buf;
	new->next = NULL;
	temp = ft_lstlast(*list);
	if (!temp)
	{
		*list = new;
		return ;
	}
	temp->next = new;
}

int	find_newline(t_list *list)
{
	int	i;

	while (list)
	{
		i = 0;
		while (((char *)list->content)[i])
		{
			if (((char *)list->content)[i] == '\n')
				return (1);
			i++;
		}
		list = list->next;
	}
	return (0);
}

int	ft_len_needed(t_list **list)
{
	int		count;
	int		i;
	t_list	*temp;

	count = 0;
	temp = *list;
	while (temp)
	{
		i = 0;
		while (((char *)temp->content)[i] && ((char *)temp->content)[i] != '\n')
		{
			i++;
			count++;
		}
		if (((char *)temp->content)[i] == '\n')
			return (count + 1);
		temp = temp->next;
	}
	return (count);
}

void	prepare_list(t_list **list, char *rest)
{
	t_list	*tmp;
	t_list	*last;

	if (!list || !*list)
		return ;
	last = ft_lstlast(*list);
	while (*list != last)
	{
		tmp = (*list)->next;
		free((*list)->content);
		free(*list);
		*list = tmp;
	}
	free(last->content);
	if (rest)
	{
		last->content = rest;
	}
	else
	{
		free(last);
		free(rest);
		*list = NULL;
	}
}

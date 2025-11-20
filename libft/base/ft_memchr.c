/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:18:30 by llechert          #+#    #+#             */
/*   Updated: 2025/06/18 21:25:05 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memchr(const void *str, int c, size_t n)
{
	size_t			i;
	unsigned char	tmp;
	unsigned char	*s;

	i = 0;
	tmp = (unsigned char)c;
	s = (unsigned char *)str;
	while (i < n)
	{
		if (s[i] == tmp)
			return ((void *)str + i);
		i++;
	}
	return (NULL);
}

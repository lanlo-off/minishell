/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 16:27:14 by llechert          #+#    #+#             */
/*   Updated: 2025/06/18 21:26:14 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!*to_find)
		return ((char *)str);
	while (str[i] && i < n)
	{
		j = 0;
		while (str[i + j] == to_find[j] && i + j < n && str[i + j])
		{
			if (j == ft_strlen(to_find) - 1)
				return ((char *)&str[i]);
			j++;
		}
		i++;
	}
	return (NULL);
}

/*#include <stdio.h>
int	main(void)
{
	printf("%s\n", ft_strnstr("hello, world !", "hello", 50));
	printf("%s\n", ft_strnstr("hello, world !", "hello", 2));
	printf("%s\n", ft_strnstr("hello, world !", "", 2));
	return (0);
}*/
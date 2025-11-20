/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:18:45 by llechert          #+#    #+#             */
/*   Updated: 2025/06/18 21:25:43 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_word_len(int index, char const *s, char c)
{
	int	j;

	j = index;
	while (s[j] && s[j] != c)
		j++;
	return (j - index);
}

static int	ft_count_word(char const *str, char c)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	while (str[i])
	{
		while (str[i] && str[i] == c)
			i++;
		if (str[i])
			n++;
		while (str[i] && str[i] != c)
			i++;
	}
	return (n);
}

static char	*ft_strncpy(char *dest, char const *src, int n)
{
	int	i;

	i = 0;
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static void	*ft_free(char **str, int j)
{
	int	i;

	i = 0;
	while (i < j)
	{
		free(str[i]);
		i++;
	}
	free(str);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	char	**res;

	res = ft_calloc(ft_count_word(s, c) + 1, sizeof(char *));
	if (res == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (j < ft_count_word(s, c))
		{
			res[j] = ft_calloc(ft_word_len(i, s, c) + 1, sizeof(char));
			if (res[j] == NULL)
				return (ft_free(res, j));
			ft_strncpy(res[j], &s[i], ft_word_len(i, s, c));
			j++;
			i += ft_word_len(i, s, c);
		}
	}
	res[j] = 0;
	return (res);
}

/*#include <stdio.h>
int	main(void)
{
	char	**res;
	char	*s = "abc de f";
	char	c = ' ';
	int		i;

	i = 0;
	res = ft_split(s, c);
	while (i < ft_count_word(s, c))
	{
		printf("%s\n", res[i]);
		i++;
	}
	ft_free(res, i);
	return (0);
}*/
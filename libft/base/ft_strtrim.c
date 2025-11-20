/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:19:41 by llechert          #+#    #+#             */
/*   Updated: 2025/06/18 21:26:21 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_is_in_set(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static char	*ft_new_str(char const *s1, int start, int end)
{
	int		len;
	int		i;
	char	*res;

	i = 0;
	len = end - start + 1;
	if (len <= 0)
		return (ft_strdup(""));
	res = ft_calloc(len + 1, sizeof(char));
	if (!res)
		return (NULL);
	while (i < len)
	{
		res[i] = s1[i + start];
		i++;
	}
	res[i] = 0;
	return (res);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		s_trim;
	int		e_trim;
	char	*str;

	s_trim = 0;
	e_trim = ft_strlen(s1) - 1;
	if (e_trim == -1)
		return (ft_strdup(""));
	while (ft_is_in_set(s1[s_trim], set))
		s_trim++;
	while (ft_is_in_set(s1[e_trim], set))
		e_trim--;
	str = ft_new_str(s1, s_trim, e_trim);
	return (str);
}

/*#include <stdio.h>
int	main(void)
{
	char	*s1 = "aaaBonjouraa\naa";
	char	*s2 = "aaaBonjouraa\naa";
	char	*s3 = "aaaBonjouraa\naa";
	char	*s4 = "aaaBonjouraa";

	printf("%s - %s\n", s1, ft_strtrim(s1, "a\n"));
	printf("%s - %s\n", s1, ft_strtrim(s2, "BCDF"));
	printf("%s - %s\n", s1, ft_strtrim(s3, ""));
	printf("%s - %s\n", s1, ft_strtrim(s4, "aBonjur\n"));
	return (0);
}*/
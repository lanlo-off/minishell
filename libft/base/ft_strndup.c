/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:48:12 by llechert          #+#    #+#             */
/*   Updated: 2025/12/03 19:00:42 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strndup(const char *s, int len)
{
	char	*dup;
	int		i;

	i = 0;
	dup = malloc(sizeof(char) * len + 1);
	if (!dup)
		return (NULL);
	while (i != len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

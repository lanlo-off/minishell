/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 16:27:20 by llechert          #+#    #+#             */
/*   Updated: 2025/06/18 21:26:18 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strrchr(const char *str, int c)
{
	int		i;
	char	*res;
	char	tmp;

	i = 0;
	res = 0;
	tmp = (char)c;
	while (str[i])
	{
		if (str[i] == tmp)
			res = (char *)&str[i];
		i++;
	}
	if (str[i] == tmp)
		res = (char *)&str[i];
	return (res);
}
//le dernier if arrive quand s[i] = 0 donc checke si c = 0 depuis le debut
//si caractere pas trouve, res reste a 0 donc renvoie bien null

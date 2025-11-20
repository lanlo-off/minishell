/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:18:27 by llechert          #+#    #+#             */
/*   Updated: 2025/06/18 21:24:33 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_int_len(long nb)
{
	long	size;

	size = 0;
	if (nb < 0)
	{
		size++;
		nb = -nb;
	}
	while (nb >= 10)
	{
		nb = nb / 10;
		size++;
	}
	return (size + 1);
}

static char	*ft_fill_itoa(char *res, long nb, int index)
{
	if (nb < 0)
	{
		res[0] = '-';
		nb = -nb;
	}
	res[index] = '\0';
	index--;
	if (nb == 0)
		res[index] = 48;
	while (nb != 0)
	{
		res[index] = (nb % 10) + 48;
		nb = nb / 10;
		index--;
	}
	return (res);
}

char	*ft_itoa(int n)
{
	long	nb;
	int		size;
	char	*res;

	nb = n;
	size = ft_int_len(nb);
	res = ft_calloc(size + 1, sizeof(char));
	if (!res)
		return (NULL);
	ft_fill_itoa(res, nb, size);
	return (res);
}

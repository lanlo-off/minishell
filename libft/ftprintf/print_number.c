/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_number.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmiotla <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 14:49:23 by mmiotla           #+#    #+#             */
/*   Updated: 2025/05/31 14:49:58 by mmiotla          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	print_number(int n)
{
	long	nb;
	int		count;
	char	c;

	nb = n;
	count = 0;
	if (nb < 0)
	{
		count += write(1, "-", 1);
		nb = -nb;
	}
	if (nb >= 10)
		count += print_number(nb / 10);
	c = nb % 10 + '0';
	count += write(1, &c, 1);
	return (count);
}

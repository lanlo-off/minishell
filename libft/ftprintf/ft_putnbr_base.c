/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:00:59 by llechert          #+#    #+#             */
/*   Updated: 2025/05/23 10:18:27 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_nbrlen_base(unsigned int nb, unsigned int base_size)
{
	int	count;

	count = 0;
	while (nb >= base_size)
	{
		nb = nb / base_size;
		count++;
	}
	count++;
	return (count);
}

int	ft_putnbr(int nb)
{
	int		count;

	count = 0;
	if (nb == -2147483648)
	{
		write(1, "-2147483648", 11);
		return (11);
	}
	if (nb < 0)
	{
		ft_putchar('-');
		count++;
		nb = -nb;
	}
	count += ft_nbrlen_base((unsigned int)nb, 10);
	if (nb > 9)
		ft_putnbr(nb / 10);
	ft_putchar("0123456789"[nb % 10]);
	return (count);
}

int	ft_putnbr_base(unsigned int nb, char c, unsigned int base_size)
{
	int	count;

	count = ft_nbrlen_base(nb, base_size);
	if (nb >= base_size)
	{
		ft_putnbr_base(nb / base_size, c, base_size);
	}
	if (c == 'x')
		ft_putchar("0123456789abcdef"[nb % base_size]);
	else
		ft_putchar("0123456789ABCDEF"[nb % base_size]);
	return (count);
}

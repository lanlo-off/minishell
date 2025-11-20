/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:31:58 by llechert          #+#    #+#             */
/*   Updated: 2025/05/14 11:52:02 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_putptr_hexa(uintptr_t nb)
{
	if (nb >= 16)
	{
		ft_putptr_hexa(nb / 16);
	}
	ft_putchar("0123456789abcdef"[nb % 16]);
}

int	ft_putptr(void *ptr)
{
	int			count;
	uintptr_t	n;

	if (!ptr)
		return (ft_putstr("(nil)"));
	n = (uintptr_t)ptr;
	count = 0;
	count += ft_putstr("0x");
	while (n >= 16)
	{
		n = n / 16;
		count++;
	}
	ft_putptr_hexa((uintptr_t)ptr);
	return (count + 1);
}

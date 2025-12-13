/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_pointer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmiotla <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 14:50:11 by mmiotla           #+#    #+#             */
/*   Updated: 2025/05/31 16:39:16 by mmiotla          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

static int	put_hex(unsigned long n)
{
	int		count;
	char	*base;

	count = 0;
	base = "0123456789abcdef";
	if (n >= 16)
		count += put_hex(n / 16);
	count += write(1, &base[n % 16], 1);
	return (count);
}

int	print_pointer(void *ptr)
{
	unsigned long	addr;
	int				count;

	addr = (unsigned long)ptr;
	count = 0;
	if (addr == 0)
		return (count + write(1, "(nil)", 5));
	count += write(1, "0x", 2);
	count += put_hex(addr);
	return (count);
}

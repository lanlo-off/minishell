/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmiotla <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 14:53:23 by mmiotla           #+#    #+#             */
/*   Updated: 2025/05/31 14:53:49 by mmiotla          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	print_hex(unsigned int n, int uppercase)
{
	int		count;
	char	*base;

	count = 0;
	if (uppercase == 1)
		base = "0123456789ABCDEF";
	else
		base = "0123456789abcdef";
	if (n >= 16)
		count += print_hex(n / 16, uppercase);
	count += write(1, &base[n % 16], 1);
	return (count);
}

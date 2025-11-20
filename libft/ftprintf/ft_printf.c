/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:00:57 by llechert          #+#    #+#             */
/*   Updated: 2025/05/07 17:01:13 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_format(char *str, int i, va_list args)
{
	if (str[i] == 'c')
		return (ft_putchar(va_arg(args, int)));
	if (str[i] == 's')
		return (ft_putstr(va_arg(args, char *)));
	if (str[i] == 'd' || str[i] == 'i')
		return (ft_putnbr(va_arg(args, int)));
	if (str[i] == 'u')
		return (ft_putnbr_base(va_arg(args, unsigned int), str[i], 10));
	if (str[i] == 'x')
		return (ft_putnbr_base(va_arg(args, unsigned int), str[i], 16));
	if (str[i] == 'X')
		return (ft_putnbr_base(va_arg(args, unsigned int), str[i], 16));
	if (str[i] == 'p')
		return (ft_putptr(va_arg(args, void *)));
	if (str[i] == '%')
		return (ft_putchar('%'));
	else
		return (0);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		i;
	int		count;

	i = 0;
	count = 0;
	va_start(args, str);
	while (str[i])
	{
		while (str[i] && str[i] != '%')
		{
			count += ft_putchar(str[i]);
			i++;
		}
		if (str[i] == '%')
		{
			i++;
			count += ft_format((char *)str, i, args);
			i++;
		}
	}
	return (count);
}

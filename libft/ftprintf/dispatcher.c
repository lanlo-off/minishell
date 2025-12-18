/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmiotla <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 14:54:44 by mmiotla           #+#    #+#             */
/*   Updated: 2025/05/31 14:56:43 by mmiotla          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	dispatcher(char spec, va_list *args)
{
	if (spec == 'c')
		return (print_char(va_arg(*args, int)));
	else if (spec == 's')
		return (print_string(va_arg(*args, char *)));
	else if (spec == 'd' || spec == 'i')
		return (print_number(va_arg(*args, int)));
	else if (spec == 'u')
		return (print_unsigned(va_arg(*args, unsigned int)));
	else if (spec == 'x')
		return (print_hex(va_arg(*args, unsigned int), 0));
	else if (spec == 'X')
		return (print_hex(va_arg(*args, unsigned int), 1));
	else if (spec == 'p')
		return (print_pointer(va_arg(*args, void *)));
	else if (spec == '%')
		return (write(1, "%", 1));
	return (0);
}

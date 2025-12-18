/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmiotla <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 14:45:44 by mmiotla           #+#    #+#             */
/*   Updated: 2025/05/31 14:45:47 by mmiotla          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int	ft_printf(const char *format, ...);
int	dispatcher(char spec, va_list *args);

int	print_char(char c);
int	print_string(char *s);
int	print_number(int n);
int	print_unsigned(unsigned int n);
int	print_hex(unsigned int n, int uppercase);
int	print_pointer(void *ptr);

#endif

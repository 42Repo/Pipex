/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fomat_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 04:46:05 by asuc              #+#    #+#             */
/*   Updated: 2023/11/13 18:18:58 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr(char *str)
{
	int	i;
	int	ret;

	ret = 0;
	i = 0;
	if (!str)
	{
		while (i < 6)
		{
			ret += ft_putchar_fd("(null)"[i], 1);
			i++;
		}
		return (ret);
	}
	if (str[0] == '\0')
		return (-1);
	while (str[i])
	{
		ret += ft_putchar_fd(str[i], 1);
		i++;
	}
	return (ret);
}

int	print_format(const char *format, int index, va_list arg)
{
	int	ret;

	ret = 0;
	if (format[index] == 'c')
		ret = ft_putchar_fd(va_arg(arg, int), 1);
	else if (format[index] == 's')
		ret = ft_putstr(va_arg(arg, char *));
	else if (format[index] == 'p')
		ret = print_hex(va_arg(arg, unsigned long));
	else if (format[index] == 'd' || format[index] == 'i')
		ret = ft_putnbr_fd(va_arg(arg, int), 1);
	else if (format[index] == 'u')
		ret = ft_putnbr_fd(va_arg(arg, unsigned int), 1);
	else if (format[index] == 'x')
		ret = print_hex_other(va_arg(arg, long), 2);
	else if (format[index] == 'X')
		ret = print_hex_other(va_arg(arg, long), 3);
	else if (format[index] == '%')
		ret = ft_putchar_fd('%', 1);
	return (ret);
}

int	is_format(char c)
{
	if (c == 'c' || c == 's' || c == 'p' || c == 'd' || c == 'i' || c == 'u'
		|| c == 'x' || c == 'X' || c == '%')
		return (1);
	return (0);
}

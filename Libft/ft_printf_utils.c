/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 17:53:40 by asuc              #+#    #+#             */
/*   Updated: 2023/11/13 18:20:31 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_hex(unsigned long nb)
{
	char	*str;
	int		ret;

	ret = 0;
	if (nb == 0)
	{
		ret = ft_putstr_fd("(nil)", 1);
		return (ret);
	}
	ret += ft_putstr_fd("0x", 1);
	str = ft_itoa_base(nb, 16);
	ret += ft_putstr_fd(str, 1);
	free(str);
	return (ret);
}

int	ft_num_len_base(unsigned long n, int base)
{
	int	len;

	len = 0;
	if (n == 0)
		return (1);
	while (n)
	{
		n /= base;
		len++;
	}
	return (len);
}

char	*ft_itoa_base(unsigned long n, int base)
{
	int		len;
	char	*str;

	if (base < 2 || base > 16)
		return (NULL);
	len = ft_num_len_base(n, base);
	str = ft_calloc(sizeof(char), (len + 1));
	if (str == NULL)
		return (NULL);
	len--;
	while (len >= 0)
	{
		str[len] = "0123456789abcdef"[n % base];
		n /= base;
		len--;
	}
	return (str);
}

char	*ft_itoa_base_hex(unsigned int n, int base, int mode)
{
	int		len;
	char	*str;

	if (base < 2 || base > 16)
		return (NULL);
	len = ft_num_len_base(n, base);
	str = ft_calloc(sizeof(char), (len + 1));
	if (str == NULL)
		return (NULL);
	len--;
	while (len >= 0)
	{
		if (mode == 3)
			str[len] = "0123456789ABCDEF"[n % base];
		else
			str[len] = "0123456789abcdef"[n % base];
		n /= base;
		len--;
	}
	return (str);
}

int	print_hex_other(unsigned int nb, int mode)
{
	char	*str;
	int		ret;

	ret = 0;
	if (nb == 0)
		return (ft_putstr_fd("0", 1));
	str = ft_itoa_base_hex(nb, 16, mode);
	ret += ft_putstr_fd(str, 1);
	free(str);
	return (ret);
}

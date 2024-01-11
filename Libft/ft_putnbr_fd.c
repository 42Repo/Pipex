/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 23:34:53 by asuc              #+#    #+#             */
/*   Updated: 2023/11/09 02:42:48 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_fd(long n, int fd)
{
	int		to_print;
	int		ret;
	long	nb;

	if (fd < 0 || fd > 2)
		return (0);
	nb = (long)n;
	ret = 0;
	if (nb < 0)
	{
		ret += write(fd, "-", 1);
		nb = -nb;
		ret += ft_putnbr_fd(nb, fd);
	}
	else
	{
		to_print = (nb % 10) + '0';
		if (nb / 10 != 0)
		{
			ret += ft_putnbr_fd(nb / 10, fd);
		}
		ret += write(fd, &to_print, 1);
		return (ret);
	}
	return (ret);
}

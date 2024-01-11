/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 23:28:48 by asuc              #+#    #+#             */
/*   Updated: 2023/11/09 02:18:11 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_numlen(long long int n)
{
	int	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		n = -n;
		len++;
	}
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static void	neg_nbr(unsigned int n, int len, char **str)
{
	(*str) = ft_calloc((len + 1), sizeof(char));
	if ((*str) == NULL)
		return ;
	(*str)[0] = '-';
	len--;
	while (len > 0)
	{
		(*str)[len] = n % 10 + '0';
		n /= 10;
		len--;
	}
}

char	*ft_itoa(int n)
{
	int		len;
	char	*str;

	if (!n)
		return (ft_strdup("0"));
	len = ft_numlen(n);
	if (n < 0)
	{
		neg_nbr(-n, len, &str);
		return (str);
	}
	str = ft_calloc(sizeof(char), (len + 1));
	if (str == NULL)
		return (NULL);
	len--;
	while (len >= 0)
	{
		str[len] = n % 10 + '0';
		n /= 10;
		len--;
	}
	return (str);
}

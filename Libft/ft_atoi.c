/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 03:46:50 by asuc              #+#    #+#             */
/*   Updated: 2023/11/16 20:36:42 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_atoi(const char *str)
{
	long	i;
	long	sign;
	long	sum;

	if (!str)
		return (0);
	i = 0;
	sign = 1;
	sum = 0;
	while ((str[i] == ' ' && str[i]) || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (!(ft_isdigit(str[i])) && !(ft_isdigit(str[i + 1])))
		return (0);
	if (str[i] == '-')
		sign *= -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && (str[i] >= '0' && str [i] <= '9'))
	{
		sum *= 10;
		sum += str[i] - 48;
		i++;
	}
	sum *= sign;
	return (sum);
}

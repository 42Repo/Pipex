/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 00:25:03 by asuc              #+#    #+#             */
/*   Updated: 2023/11/09 02:47:45 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *p1, const void *p2, size_t size)
{
	int	i;
	int	res;

	if (!p1 || !p2)
		return (0);
	i = 0;
	res = 0;
	while (size)
	{
		if (((unsigned char *)p1)[i] != ((unsigned char *)p2)[i])
		{
			res = (((unsigned char *)p1)[i]) - (((unsigned char *)p2)[i]);
			return (res);
		}
		i++;
		size--;
	}
	return (0);
}

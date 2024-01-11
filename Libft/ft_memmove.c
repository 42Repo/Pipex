/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 00:25:11 by asuc              #+#    #+#             */
/*   Updated: 2023/11/09 02:47:24 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*from;
	unsigned char	*to;

	if (!dest || !src)
		return (NULL);
	from = (unsigned char *)src;
	to = (unsigned char *)dest;
	i = 0;
	if (to > from)
	{
		while (n--)
			to[n] = from[n];
	}
	else
	{
		while (i < n)
		{
			to[i] = from[i];
			i++;
		}
	}
	return (dest);
}

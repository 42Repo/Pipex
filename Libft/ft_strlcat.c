/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 18:00:41 by asuc              #+#    #+#             */
/*   Updated: 2023/11/02 18:00:41 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	j = 0;
	i = 0;
	if ((dst == NULL || src == NULL) && (size <= 0))
		return (i + j);
	while (dst[i] && i < size)
		i++;
	while (src[j] && size != 0 && i + j < size - 1)
	{
		dst[j + i] = src[j];
		j++;
	}
	if (i < size)
		dst[i + j] = 0;
	j = ft_strlen(src);
	return (i + j);
}

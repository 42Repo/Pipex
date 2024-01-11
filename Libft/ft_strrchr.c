/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 00:25:33 by asuc              #+#    #+#             */
/*   Updated: 2023/11/09 02:28:09 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;
	int	returned;

	if (!s)
		return (NULL);
	returned = -1;
	i = 0;
	while (s[i])
	{
		if (s[i] == (unsigned char)c)
			returned = i;
		i++;
	}
	if (s[i] == c)
		returned = i;
	if (returned >= 0)
		return ((char *)(s + returned));
	return (NULL);
}

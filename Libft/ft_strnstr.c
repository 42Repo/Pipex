/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 00:25:29 by asuc              #+#    #+#             */
/*   Updated: 2023/11/09 02:28:01 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	long unsigned int	i;
	long unsigned int	j;

	i = 0;
	if (!big)
		return (NULL);
	if (little[0] == 0 || !little)
		return ((char *)big);
	if (len == 0)
		return (NULL);
	while (big[i] && i < len)
	{
		j = 0;
		while (big[i + j] == little[j]
			&& little[j] && big[i + j] && i + j < len)
			j++;
		if (little[j] == 0 && big[i + j - 1] == little[j - 1])
			return ((char *) &big[i]);
		i++;
	}
	return (NULL);
}

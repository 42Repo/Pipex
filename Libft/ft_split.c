/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 18:51:56 by asuc              #+#    #+#             */
/*   Updated: 2023/11/09 02:26:18 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strdup_split(const char *src, char charset)
{
	char	*tab;
	int		i;

	i = 0;
	while (src[i] != charset && src[i])
		i++;
	tab = malloc(sizeof(char) * (i + 1));
	if (tab == NULL || src == NULL)
		return (tab);
	i = 0;
	while (src[i] && src[i] != charset)
	{
		tab[i] = src[i];
		i++;
	}
	tab[i] = '\0';
	return (tab);
}

static int	count_word(const char *str, char charset)
{
	int	i;
	int	count;
	int	bol;

	count = 0;
	i = 0;
	bol = 1;
	while (str[i])
	{
		if (str[i] != charset)
		{
			if (bol == 1)
				count++;
			bol = 0;
		}
		else
			bol = 1;
		i++;
	}
	return (count);
}

char	**ft_split(const char *str, char charset)
{
	char	**ret;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	ret = NULL;
	j = 0;
	i = 0;
	if (!str)
		return (NULL);
	ret = malloc(sizeof(char *) * (count_word(str, charset) + 1));
	if (ret == NULL)
		return (ret);
	while (i < count_word(str, charset))
	{
		while (str[j] == charset && str[j] != 0)
			j++;
		ret[i] = ft_strdup_split(str + j, charset);
		i++;
		while (str[j] != charset && str[j] != 0)
			j++;
	}
	ret[i] = 0;
	return (ret);
}

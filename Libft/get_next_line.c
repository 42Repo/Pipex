/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 19:51:27 by asuc              #+#    #+#             */
/*   Updated: 2024/01/21 20:10:06 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_null(char **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

static char	*join_line(char **buffer, ssize_t nl_index)
{
	char	*line;
	char	*new_buffer;

	if (nl_index >= 0)
	{
		line = ft_substr(*buffer, 0, nl_index + 1);
		if ((*buffer)[nl_index + 1] == '\0')
			new_buffer = NULL;
		else
			new_buffer = ft_strdup(*buffer + nl_index + 1);
		free_null(buffer);
		*buffer = new_buffer;
	}
	else
	{
		if (!(*buffer)[0])
		{
			free_null(buffer);
			return (NULL);
		}
		line = ft_strdup(*buffer);
		free_null(buffer);
	}
	return (line);
}

static char	*read_line(int fd, char **buffer)
{
	char	*read_buffer;
	ssize_t	bytes_read;
	char	*temp;
	char	*nl_ptr;

	read_buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (read_buffer == NULL)
		return (NULL);
	nl_ptr = ft_strchr(*buffer, '\n');
	while (!nl_ptr)
	{
		bytes_read = read(fd, read_buffer, BUFFER_SIZE);
		if (bytes_read <= 0 && bytes_read != -1)
		{
			free(read_buffer);
			return (join_line(buffer, -1));
		}
		read_buffer[bytes_read] = '\0';
		temp = ft_strjoin(*buffer, read_buffer);
		free_null(buffer);
		*buffer = temp;
		nl_ptr = ft_strchr(*buffer, '\n');
	}
	free(read_buffer);
	return (join_line(buffer, nl_ptr - *buffer));
}

char	*get_next_line(int fd)
{
	static char	*buffer[OPEN_MAX];

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > OPEN_MAX)
		return (NULL);
	if (!buffer[fd])
		buffer[fd] = ft_strdup("");
	return (read_line(fd, &buffer[fd]));
}

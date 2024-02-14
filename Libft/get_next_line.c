/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 19:51:27 by asuc              #+#    #+#             */
/*   Updated: 2024/02/14 12:32:05 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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

static char	*init_read_buffer(void)
{
	char	*read_buffer;

	read_buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!read_buffer)
		return (NULL);
	return (read_buffer);
}

static int	handle_read(ssize_t bytes_read, char **read_buffer, char **buffer)
{
	if (*buffer[0] == '\0' && bytes_read == 0)
	{
		free(*read_buffer);
		return (0);
	}
	if (bytes_read < 0)
	{
		free(*read_buffer);
		return (0);
	}
	return (1);
}

char	*read_line(int fd, char **buffer)
{
	char	*read_buffer;
	ssize_t	bytes_read;
	char	*temp;

	read_buffer = init_read_buffer();
	if (!read_buffer)
		return (NULL);
	while (!(ft_strchr(*buffer, '\n')))
	{
		bytes_read = read(fd, read_buffer, BUFFER_SIZE);
		if (handle_read(bytes_read, &read_buffer, buffer) != 1)
		{
			if (bytes_read < 0)
				return (NULL);
			else
				return (join_line(buffer, -1));
		}
		read_buffer[bytes_read] = '\0';
		temp = ft_strjoin(*buffer, read_buffer);
		free_null(buffer);
		*buffer = temp;
	}
	free(read_buffer);
	return (join_line(buffer, ft_strchr(*buffer, '\n') - *buffer));
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

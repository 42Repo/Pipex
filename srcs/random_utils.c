/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 21:05:35 by asuc              #+#    #+#             */
/*   Updated: 2024/01/26 21:22:48 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	random_init(t_pipex *pipex)
{
	int		urandom_fd;
	int		bytes_read;
	char	buffer[2];
	int		i;

	i = 1;
	urandom_fd = open("/dev/urandom", O_RDONLY);
	if (urandom_fd == -1)
		error_exit_fd("Error", -1);
	while (i < 20)
	{
		bytes_read = read(urandom_fd, buffer, 1);
		if (bytes_read == -1)
			error_exit_fd("Error", urandom_fd);
		while (ft_isalnum(buffer[0]) == 0)
			buffer[0] = (buffer[0] % 93) + 33;
		pipex->random_name[i] = buffer[0];
		i++;
	}
	pipex->random_name[0] = '.';
	pipex->random_name[i] = '\0';
	close(urandom_fd);
}

void	error_exit_invalid(t_pipex *pipex_p, int pipefd[][2], int fd)
{
	if (fd > 0)
		close(fd);
	close((*pipefd)[0]);
	close((*pipefd)[1]);
	clean_pipex(pipex_p, 1);
	perror("Error");
	exit(EXIT_FAILURE);
}

void	error_exit_fd(char *error_msg, int fd)
{
	if (fd > 0)
		close(fd);
	perror(error_msg);
	exit(EXIT_FAILURE);
}

int	ft_tablen(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

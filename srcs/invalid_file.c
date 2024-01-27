/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invalid_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 21:11:49 by asuc              #+#    #+#             */
/*   Updated: 2024/01/26 21:53:28 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static void	invalid_infile(t_pipex *pipex_p, int pipefd[][2])
{
	int		bytes_read;
	char	buffer[MAX_URANDOM_BYTES];
	int		urandom_fd;

	if (pipex_p->random == file_random)
		urandom_fd = open("/dev/random", O_RDONLY);
	else
		urandom_fd = open("/dev/urandom", O_RDONLY);
	if (urandom_fd == -1)
		error_exit_invalid(pipex_p, pipefd, -1);
	bytes_read = read(urandom_fd, buffer, MAX_URANDOM_BYTES);
	if (bytes_read == -1)
		error_exit_invalid(pipex_p, pipefd, urandom_fd);
	close(pipex_p->in_fd);
	pipex_p->in_fd = open(pipex_p->random_name, O_CREAT | O_RDWR | O_TRUNC,
			0644);
	if (pipex_p->in_fd == -1)
		error_exit_invalid(pipex_p, pipefd, urandom_fd);
	if (write(pipex_p->in_fd, buffer, bytes_read) == -1)
		error_exit_invalid(pipex_p, pipefd, urandom_fd);
	close(urandom_fd);
	close(pipex_p->in_fd);
}

void	invalid_file_exec(t_pipex *pipex_p, int i, char **envp, int pipefd[][2])
{
	invalid_infile(pipex_p, pipefd);
	pipex_p->in_fd = open(pipex_p->random_name, O_RDONLY);
	dup2(pipex_p->in_fd, STDIN_FILENO);
	dup2((*pipefd)[1], STDOUT_FILENO);
	close((*pipefd)[1]);
	close((*pipefd)[0]);
	close(pipex_p->in_fd);
	close(pipex_p->out_fd);
	execve(pipex_p->cmd_paths[i], pipex_p->cmd_args[i], envp);
	perror("execve");
	close(0);
	close(1);
	close(2);
	clean_pipex(pipex_p, 1);
	exit(EXIT_FAILURE);
}

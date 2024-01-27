/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 21:10:21 by asuc              #+#    #+#             */
/*   Updated: 2024/01/26 21:53:43 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static void	handle_here_doc_failure(t_pipex *pipex_p, int pipefd[][2])
{
	close(pipefd[0][0]);
	close(pipefd[0][1]);
	close(0);
	close(1);
	close(2);
	clean_pipex(pipex_p, 1);
	exit(EXIT_FAILURE);
}

static void	setup_file_descriptors(t_pipex *pipex_p, int i, int pipefd[][2])
{
	close(pipex_p->in_fd);
	pipex_p->in_fd = open(pipex_p->random_name, O_RDONLY);
	dup2(pipex_p->in_fd, STDIN_FILENO);
	if (i == pipex_p->cmd_count - 1)
		dup2(pipex_p->out_fd, STDOUT_FILENO);
	else
		dup2(pipefd[0][1], STDOUT_FILENO);
	close(pipefd[0][1]);
	close(pipefd[0][0]);
	close(pipex_p->in_fd);
	close(pipex_p->out_fd);
}

static void	execute_command_or_fail(t_pipex *pipex_p, int i, char **envp)
{
	if (check_exec_command(pipex_p, i) == 0)
		execve(pipex_p->cmd_paths[i], pipex_p->cmd_args[i], envp);
	close(0);
	close(1);
	close(2);
	clean_pipex(pipex_p, 1);
	perror("execve");
	exit(EXIT_FAILURE);
}

int	here_doc_exec(t_pipex *pipex_p, int i, char **envp, int pipefd[][2])
{
	if (here_doc(pipex_p) == -1)
		handle_here_doc_failure(pipex_p, pipefd);
	setup_file_descriptors(pipex_p, i, pipefd);
	execute_command_or_fail(pipex_p, i, envp);
	return (0);
}

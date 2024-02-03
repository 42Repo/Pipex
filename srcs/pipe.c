/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 21:27:45 by asuc              #+#    #+#             */
/*   Updated: 2024/02/03 03:04:24 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static void	pipe_exec(t_pipex *pipex_p, int i, char **envp, int pipefd[][2])
{
	dup2(pipex_p->in_fd, STDIN_FILENO);
	if (i == pipex_p->cmd_count - 1)
		dup2(pipex_p->out_fd, STDOUT_FILENO);
	else
		dup2((*pipefd)[1], STDOUT_FILENO);
	close((*pipefd)[0]);
	close((*pipefd)[1]);
	close(pipex_p->in_fd);
	close(pipex_p->out_fd);
	if (check_exec_command(pipex_p, i) == 0)
		execve(pipex_p->cmd_paths[i], pipex_p->cmd_args[i], envp);
	close(0);
	close(1);
	close(2);
	clean_pipex(pipex_p, 1);
	perror("execve");
	exit(EXIT_FAILURE);
}

static void	child_exec(t_pipex *pipex_p, int i, char **envp, int pipefd[][2])
{
	if (i == 0)
	{
		if (pipex_p->here_doc == true)
			here_doc_exec(pipex_p, i, envp, pipefd);
		else if (pipex_p->is_invalid_infile == true)
			invalid_file_exec(pipex_p, i, envp, pipefd);
		else
			pipe_first_exec(pipex_p, i, envp, pipefd);
	}
	else
		pipe_exec(pipex_p, i, envp, pipefd);
}

int	check_exec_command(t_pipex *pipex_p, int i)
{
	if (pipex_p->cmd_paths[i] != NULL)
	{
		if (access(pipex_p->cmd_paths[i], F_OK) == -1
			|| access(pipex_p->cmd_paths[i], X_OK) == -1)
		{
			perror("Error");
			return (-1);
		}
		return (0);
	}
	return (-1);
}

int	exec_pipex(t_pipex *pipex_p, int i, char **envp)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		return (error_pipex("pipe"));
	pipex_p->pid[i] = fork();
	if (pipex_p->pid[i] == -1)
		return (error_pipex("fork"));
	if (pipex_p->pid[i] == 0)
		child_exec(pipex_p, i, envp, &pipefd);
	else
	{
		close(pipefd[1]);
		close(pipex_p->in_fd);
		pipex_p->in_fd = dup(pipefd[0]);
		close(pipefd[0]);
	}
	return (0);
}

void	pipe_first_exec(t_pipex *pipex_p, int i, char **envp, int pipefd[][2])
{
	dup2(pipex_p->in_fd, STDIN_FILENO);
	dup2((*pipefd)[1], STDOUT_FILENO);
	close((*pipefd)[0]);
	close((*pipefd)[1]);
	close(pipex_p->in_fd);
	close(pipex_p->out_fd);
	if (check_exec_command(pipex_p, i) == 0)
		execve(pipex_p->cmd_paths[i], pipex_p->cmd_args[i], envp);
	close(0);
	close(1);
	close(2);
	clean_pipex(pipex_p, 1);
	perror("execve");
	exit(EXIT_FAILURE);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 19:42:09 by asuc              #+#    #+#             */
/*   Updated: 2024/01/23 22:03:10 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

#define MAX_URANDOM_BYTES 65536

int	clean_pipex(t_pipex *pipex_p, int mode)
{
	int	i;

	i = 0;
	free(pipex_p->pid);
	while (i < pipex_p->cmd_count)
	{
		free(pipex_p->cmd_paths[i]);
		i++;
	}
	free(pipex_p->cmd_paths);
	if (pipex_p->limiter != NULL)
		free(pipex_p->limiter);
	close_all(pipex_p);
	if (mode == 0)
		return (0);
	i = 0;
	while (i < pipex_p->cmd_count)
	{
		free_tab(&pipex_p->cmd_args[i]);
		i++;
	}
	free(pipex_p->cmd_args);
	return (0);
}

int	parse_args(t_pipex *pipex_p, char **ag, int ac)
{
	int	i;
	int	j;

	i = 2;
	j = 0;
	pipex_p->cmd_args = ft_calloc(sizeof(char **), pipex_p->cmd_count + 1);
	if (pipex_p->cmd_args == NULL)
		return (-1);
	if (pipex_p->here_doc == true)
		i = 3;
	while (i < ac - 1)
	{
		pipex_p->cmd_args[j] = ft_split(ag[i], ' ');
		if (pipex_p->cmd_args[j] == NULL)
			return (-1);
		i++;
		j++;
	}
	return (0);
}

int	test_open(t_pipex *pipex_p)
{
	if (access(pipex_p->random_name, F_OK) == -1)
	{
		perror("Error tmp file");
		return (-1);
	}
	if (access(pipex_p->random_name, W_OK) == -1)
	{
		perror("Error tmp file");
		return (-1);
	}
	return (0);
}

int	here_doc(t_pipex *pipex)
{
	char	*line;
	int		bol;

	bol = 0;
	line = ft_strdup("");
	while (1)
	{
		free(line);
		line = NULL;
		if (test_open(pipex) == -1)
			return (-1);
		if (bol == 0)
		{
			ft_printf("pipe heredoc> ");
			bol = 1;
		}
		line = get_next_line(0);
		if (line != NULL && ft_strlen(line) == ft_strlen(pipex->limiter)
			&& ft_strncmp(line, pipex->limiter, ft_strlen(pipex->limiter)) == 0)
		{
			free(line);
			line = NULL;
			break ;
		}
		if (line != NULL)
		{
			if (ft_strlen(line) >= 1 && line[ft_strlen(line) - 1] == '\n')
				bol = 0;
			ft_putendl_fd(line, pipex->in_fd);
		}
		if (line == NULL)
			line = ft_strdup("");
	}
	if (line != NULL)
		free(line);
	return (0);
}

void	error_exit_invalid(t_pipex *pipex_p, int pipefd[][2], int fd)
{
	if (fd > 0)
		close(fd);
	close((*pipefd)[0]);
	close((*pipefd)[1]);
	clean_pipex(pipex_p, 1);
	perror("Error3");
	exit(EXIT_FAILURE);
}

void	invalid_infile(t_pipex *pipex_p, int pipefd[][2])
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
	pipex_p->in_fd = open(pipex_p->random_name,
			O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (pipex_p->in_fd == -1)
		error_exit_invalid(pipex_p, pipefd, urandom_fd);
	if (write(pipex_p->in_fd, buffer, bytes_read) == -1)
		error_exit_invalid(pipex_p, pipefd, urandom_fd);
	close(urandom_fd);
	close(pipex_p->in_fd);
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

int	here_doc_exec(t_pipex *pipex_p, int i, char **envp, int pipefd[][2])
{
	if (here_doc(pipex_p) == -1)
	{
		close((*pipefd)[0]);
		close((*pipefd)[1]);
		close(0);
		close(1);
		close(2);
		clean_pipex(pipex_p, 1);
		exit(EXIT_FAILURE);
	}
	close(pipex_p->in_fd);
	pipex_p->in_fd = open(pipex_p->random_name, O_RDONLY);
	dup2(pipex_p->in_fd, STDIN_FILENO);
	if (i == pipex_p->cmd_count - 1)
		dup2(pipex_p->out_fd, STDOUT_FILENO);
	else
		dup2((*pipefd)[1], STDOUT_FILENO);
	close((*pipefd)[1]);
	close((*pipefd)[0]);
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

int	error_pipex(char *error_msg)
{
	perror(error_msg);
	return (-1);
}

void	pipe_exec(t_pipex *pipex_p, int i, char **envp, int pipefd[][2])
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

void	child_exec(t_pipex *pipex_p, int i, char **envp, int pipefd[][2])
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

int	exec_pipex(t_pipex *pipex_p, int i, char **envp)
{
	int		pipefd[2];

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

// on check si la commande que l'on veut executer existe
int	check_exec_command(t_pipex *pipex_p, int i)
{
	if (pipex_p->cmd_paths[i] != NULL)
	{
		if (access(pipex_p->cmd_paths[i], F_OK) == -1
			|| access(pipex_p->cmd_paths[i], X_OK) == -1)
		{
			perror("Error5");
			return (-1);
		}
		return (0);
	}
	return (-1);
}

int	main(int ac, char **ag, char **envp)
{
	t_pipex	pipex_p;
	int		i;

	i = 0;
	init_pipex(&pipex_p);
	pipex_p.pid = malloc(sizeof(pid_t) * (ac - 3));
	if (pipex_p.pid == NULL)
		return (0);
	if (check_args(&pipex_p, ac, ag) == -1)
		return (clean_pipex(&pipex_p, 1));
	if (pipex_p.here_doc == false)
	{
		if (parse_cmds(&pipex_p, ag, ac, envp) == -1)
			return (clean_pipex(&pipex_p, 0));
	}
	else
	{
		if (parse_cmds(&pipex_p, ag + 1, ac - 1, envp) == -1)
			return (clean_pipex(&pipex_p, 0));
	}
	if (parse_args(&pipex_p, ag, ac) == -1)
		return (clean_pipex(&pipex_p, 1));
	while (i < pipex_p.cmd_count)
	{
		if (exec_pipex(&pipex_p, i, envp) == -1)
			return (clean_pipex(&pipex_p, 1));
		i++;
	}
	i = 0;
	while (i < pipex_p.cmd_count)
	{
		waitpid(pipex_p.pid[i], NULL, 0);
		i++;
	}
	unlink(pipex_p.random_name);
	clean_pipex(&pipex_p, 1);
	return (0);
}

char	**add_slash(char **cmd_args)
{
	int		i;
	char	*tmp;

	i = 0;
	while (cmd_args[i])
	{
		tmp = ft_strjoin(cmd_args[i], "/");
		if (tmp == NULL)
			return (NULL);
		free(cmd_args[i]);
		cmd_args[i] = ft_strdup(tmp);
		if (cmd_args[i] == NULL)
			return (NULL);
		free(tmp);
		i++;
	}
	return (cmd_args);
}

char	**find_path(char **envp)
{
	int		i;
	char	*path;
	char	**path_split;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			break ;
		i++;
	}
	if (envp[i] == NULL)
		return (NULL);
	path = ft_substr(envp[i], 5, ft_strlen(envp[i]));
	if (path == NULL)
		return (NULL);
	path_split = ft_split(path, ':');
	free(path);
	if (path_split == NULL)
		return (NULL);
	return (path_split);
}

char	**copy_tab(char **tab)
{
	int		i;
	char	**new_tab;

	i = 0;
	while (tab[i])
		i++;
	new_tab = malloc(sizeof(char *) * (i + 1));
	if (new_tab == NULL)
		return (NULL);
	i = 0;
	while (tab[i])
	{
		new_tab[i] = ft_strdup(tab[i]);
		if (new_tab[i] == NULL)
			return (NULL);
		i++;
	}
	new_tab[i] = NULL;
	return (new_tab);
}

void	free_tab(char ***tab)
{
	int	i;

	i = 0;
	while ((*tab)[i] != NULL)
	{
		free((*tab)[i]);
		i++;
	}
	free(*tab);
}

int	ft_tablen(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

// on recupere tous les chemins dans le path des commandes dont on a besoin
int	parse_cmds(t_pipex *pipex_p, char **ag, int ac, char **envp)
{
	int		i;
	char	**tmp_path;
	int		j;
	char	**path;
	char	**ag2;

	j = 2;
	i = 0;
	pipex_p->cmd_count = 0;
	path = find_path(envp);
	if (path == NULL)
		return (-1);
	path = add_slash(path);
	if (path == NULL)
		return (-1);
	pipex_p->cmd_paths = ft_calloc(sizeof(char *), (ac - 2));
	if (pipex_p->cmd_paths == NULL)
		return (-1);
	while (j < ac - 1)
	{
		i = 0;
		ag2 = ft_split(ag[j], ' ');
		tmp_path = ft_calloc(sizeof(char *), (ft_tablen(path) + 1));
		if (tmp_path == NULL)
			return (-1);
		while (path[i])
		{
			tmp_path[i] = ft_strjoin(path[i], ag2[0]);
			i++;
		}
		free_tab(&ag2);
		i = 0;
		while (tmp_path[i])
		{
			if (access(ag[j], F_OK) == 0)
			{
				if (access(ag[j], X_OK) == 0)
				{
					pipex_p->cmd_paths[pipex_p->cmd_count] = ft_strdup(ag[j]);
					if (pipex_p->cmd_paths[pipex_p->cmd_count] == NULL)
						return (-1);
					break ;
				}
			}
			if (access(tmp_path[i], F_OK) == 0)
			{
				if (access(tmp_path[i], X_OK) == 0)
				{
					pipex_p->cmd_paths[pipex_p->cmd_count] = \
						ft_strdup(tmp_path[i]);
					if (pipex_p->cmd_paths[pipex_p->cmd_count] == NULL)
						return (-1);
					break ;
				}
			}
			i++;
		}
		free_tab(&tmp_path);
		pipex_p->cmd_count++;
		j++;
	}
	pipex_p->cmd_paths[pipex_p->cmd_count] = NULL;
	free_tab(&path);
	return (0);
}

int	close_all(t_pipex *pipex_p)
{
	if (pipex_p->in_fd > 0)
		close(pipex_p->in_fd);
	if (pipex_p->out_fd > 0)
		close(pipex_p->out_fd);
	return (0);
}

void	error_exit_fd(char *error_msg, int fd)
{
	if (fd > 0)
		close(fd);
	perror(error_msg);
	exit(EXIT_FAILURE);
}

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

int	check_args(t_pipex *pipex_p, int ac, char **ag)
{
	int		f1;
	int		f2;
	char	*limiter;

	if (ac < 5)
		return (-1);
	if (ft_strncmp(ag[1], "here_doc", 8) == 0)
	{
		pipex_p->here_doc = true;
		pipex_p->in_fd = open(pipex_p->random_name,
				O_CREAT | O_RDWR | O_TRUNC, 0644);
		limiter = ft_strdup(ag[2]);
		if (limiter == NULL)
			return (-1);
		pipex_p->limiter = ft_strjoin(limiter, "\n");
		if (pipex_p->limiter == NULL)
			return (-1);
		free(limiter);
		pipex_p->out_fd = open(ag[ac - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
		return (0);
	}
	if (ft_strncmp(ag[1], "/dev/urandom", ft_strlen(ag[1])) == 0
		|| ft_strncmp(ag[1], "/dev/random", ft_strlen(ag[1])) == 0)
		pipex_p->is_invalid_infile = true;
	if (ft_strncmp(ag[1], "/dev/urandom", ft_strlen(ag[1])) == 0)
		pipex_p->random = file_urandom;
	if (ft_strncmp(ag[1], "/dev/random", ft_strlen(ag[1])) == 0)
		pipex_p->random = file_random;
	f1 = open(ag[1], O_RDONLY);
	f2 = open(ag[ac - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
	pipex_p->in_fd = f1;
	pipex_p->out_fd = f2;
	if (f1 < 0 || f2 < 0)
	{
		perror("Error opening file");
		unlink(ag[ac - 1]);
		close_all(pipex_p);
		return (-1);
	}
	return (0);
}

int	*init_pipex(t_pipex *pipex)
{
	if (pipex == NULL)
		return (NULL);
	pipex->in_fd = 0;
	pipex->out_fd = 0;
	pipex->here_doc = false;
	pipex->is_invalid_infile = false;
	pipex->cmd_paths = NULL;
	pipex->cmd_args = NULL;
	pipex->cmd_count = 0;
	pipex->limiter = NULL;
	pipex->random = file_urandom;
	pipex->pid = NULL;
	random_init(pipex);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 19:42:09 by asuc              #+#    #+#             */
/*   Updated: 2024/01/17 02:15:54 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

// int	pipex(char **argv, char **envp);
int	*init_pipex(t_pipex *pipex);
int	check_args(t_pipex *pipex_p, int ac, char **ag);
int	parse_cmds(t_pipex *pipex_p, char **ag, int ac, char **envp);

int	clean_pipex(t_pipex *pipex_p)
{
	int	i;

	i = 0;
	while (i < pipex_p->cmd_count)
	{
		free(pipex_p->cmd_paths[i]);
		i++;
	}
	free(pipex_p->cmd_paths);
	return (0);
}

// for ["/bin/cat", "/usr/bin/head", "/usr/bin/wc"], the ft_parse_args will use ft_split to yield a 2D array like this one: [["cat"], ["head", "-n", "5"], ["wc", "-l"]] (remember to NULL terminate your arrays!).
int	parse_args(t_pipex *pipex_p, char **ag, int ac)
{
	int		i;
	int		j;

	i = 2;
	j = 0;
	pipex_p->cmd_args = ft_calloc(sizeof(char **) , pipex_p->cmd_count + 1);
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

int	exec_pipex(t_pipex *pipex_p, int i)
{
	int		pid;
	int		status;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	printf("statusdeb = \n");
	if (pid == 0)
	{
		if (i == 0)
		{
			if (pipex_p->here_doc == true)
			{
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[0]);
				close(pipefd[1]);
				execve("/bin/cat", pipex_p->cmd_args[i], NULL);
			}
			else
			{
				dup2(pipex_p->in_fd, STDIN_FILENO);
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[0]);
				close(pipefd[1]);
				execve(pipex_p->cmd_paths[i], pipex_p->cmd_args[i], NULL);
			}
		}
		else if (i == pipex_p->cmd_count - 1)
		{
			// printf("statusin = \n");
			if (pipex_p->is_invalid_infile == true)
			{
				dup2(pipefd[0], STDIN_FILENO);
				close(pipefd[0]);
				close(pipefd[1]);
				execve("/usr/bin/wc", pipex_p->cmd_args[i], NULL);
			}
			else
			{
				dup2(pipefd[0], STDIN_FILENO);
				printf("statusin2 = \n");
				
				printf("pipex_p->cmd_paths[i] = %s\n", pipex_p->cmd_paths[i]);
				close(pipefd[0]);
				close(pipefd[1]);
				execve(pipex_p->cmd_paths[i], pipex_p->cmd_args[i], NULL);
			}
		}
		else
		{
			dup2(pipefd[0], STDIN_FILENO);
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[0]);
			close(pipefd[1]);
			execve(pipex_p->cmd_paths[i], pipex_p->cmd_args[i], NULL);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		close(pipefd[0]);
		close(pipefd[1]);
	}
	printf("statusfim = \n");
	return (0);
}

int	main(int ac, char **ag, char **envp)
{
	t_pipex	pipex_p;
	int		i;

	i = 0;
	init_pipex(&pipex_p);
	if (check_args(&pipex_p, ac, ag) == -1)
		return (-1);
	if (parse_cmds(&pipex_p, ag, ac, envp) == -1)
		return (-1);
	if (parse_args(&pipex_p, ag, ac) == -1)
		return (-1);
	while (i < pipex_p.cmd_count)
	{
		exec_pipex(&pipex_p, i);
		i++;
		printf("i = %d\n", i);
	}
	// pipex(ag, envp);
	clean_pipex(&pipex_p);
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
		free(cmd_args[i]);
		cmd_args[i] = ft_strdup(tmp);
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
	path = add_slash(path);
	if (path == NULL)
		return (-1);
	pipex_p->cmd_paths = ft_calloc(sizeof(char *) , (ac - 2));
	while (j < ac - 1)
	{
		i = 0;
		ag2 = ft_split(ag[j], ' ');
		tmp_path = ft_calloc(sizeof(char *), (ft_tablen(path) + 1));
		while (path[i])
		{
			tmp_path[i] = ft_strjoin(path[i], ag2[0]);
			i++;
		}
		free_tab(&ag2);
		i = 0;
		while (tmp_path[i])
		{
			if (access(tmp_path[i], F_OK) == 0)
			{
				if (access(tmp_path[i], X_OK) == 0)
				{
					pipex_p->cmd_paths[pipex_p->cmd_count] = \
							ft_strdup(tmp_path[i]);
					break ;
				}
			}
			i++;
		}
		if (pipex_p->cmd_paths[pipex_p->cmd_count] == NULL)
		{
			ft_printf("%s: command not found\n", ag[j]);
			return (-1);
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

int	check_args(t_pipex	*pipex_p, int ac, char **ag)
{
	int	f1;
	int	f2;

	if (ac < 5)
	{
		ft_printf("Error: missing arguments\n");
		return (-1);
	}
	if (ft_strncmp(ag[1], "here_doc", 8) == 0)
		pipex_p->here_doc = true;
	if (ft_strncmp(ag[1], "/dev/urandom", ft_strlen(ag[1])) == 0)
		pipex_p->is_invalid_infile = true;
	f1 = open(ag[1], O_RDONLY);
	f2 = open(ag[ac - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
	pipex_p->in_fd = f1;
	pipex_p->out_fd = f2;
	if (f1 < 0 || f2 < 0)
	{
		ft_printf("Error: invalid file\n");
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
	return (0);
}

// int	pipex(char **argv, char **envp)
// {
// 	int		i;
// 	char	*path;
// 	char	**path_split;

// 	i = 0;

// 	i = 0;
// 	while (path_split[i])
// 	{
// 		if (access(path_split[i], F_OK) == 0)
// 			break ;
// 		i++;
// 	}
// 	if (path_split[i] == NULL)
// 	{
// 		ft_printf("%s: command not found\n", argv[1]);
// 		return (0);
// 	}
// 	if (execve(path_split[i], argv + 1, envp) == 0)
// 		perror("execve error\n");
// 	return (0);
// }

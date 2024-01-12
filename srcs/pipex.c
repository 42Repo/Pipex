/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 19:42:09 by asuc              #+#    #+#             */
/*   Updated: 2024/01/12 00:15:24 by asuc             ###   ########.fr       */
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
	free(pipex_p);
	return (0);
}

int	main(int ac, char **ag, char **envp)
{
	t_pipex	pipex_p;

	init_pipex(&pipex_p);
	if (check_args(&pipex_p, ac, ag) == -1)
		return (-1);
	if (parse_cmds(&pipex_p, ag, ac, envp) == -1)
		return (-1);
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
	new_tab = malloc(sizeof(char *) * (i + 2));
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

// on recupere tous les chemins dans le path des commandes dont on a besoin
int	parse_cmds(t_pipex *pipex_p, char **ag, int ac, char **envp)
{
	int		i;
	char	**tmp_path;
	int		j;
	char	**path;

	j = 2;
	i = 0;
	pipex_p->cmd_count = 0;
	path = find_path(envp);
	path = add_slash(path);
	if (path == NULL)
		return (-1);
	pipex_p->cmd_paths = malloc(sizeof(char *) * (ac - 3));
	while (j < ac - 1)
	{
		i = 0;
		tmp_path = copy_tab(path);
		while (tmp_path[i])
		{
			tmp_path[i] = ft_strjoin(path[i], ag[j]);
			i++;
		}
		i = 0;
		while (tmp_path[i])
		{
			ft_printf("tmp_path[%d] = %s\n", i, tmp_path[i]);
			ft_printf("ag[%d] = %s\n", j, ag[j]);
			i++;
		}
		i = 0;
		while (tmp_path[i])
		{
			if (access(tmp_path[i], F_OK) == 0)
				pipex_p->cmd_paths[pipex_p->cmd_count] = ft_strdup(tmp_path[i]);
			i++;
		}
		if (pipex_p->cmd_paths[pipex_p->cmd_count] == NULL)
		{
			ft_printf("%s: command not found\n", ag[j]);
			return (-1);
		}
		pipex_p->cmd_count++;
		j++;
		i = 0;
		while (tmp_path[i])
		{
			free(tmp_path[i]);
			i++;
		}
	}
	i = 0;
	while (i < pipex_p->cmd_count)
	{
		ft_printf("cmd_paths[%d] = %s\n", i, pipex_p->cmd_paths[i]);
		i++;
	}
	ft_printf("cmd_count = %d\n", pipex_p->cmd_count);
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
	pipex = malloc(sizeof(t_pipex));
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

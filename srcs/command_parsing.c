/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 21:06:10 by asuc              #+#    #+#             */
/*   Updated: 2024/02/03 02:39:31 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static char	**free_all(char ***cmd_args, int i)
{

	while (i >= 0)
	{
		free((*cmd_args)[i]);
		i--;
	}
	free((*cmd_args));
	return (NULL);
}

static char	**create_tmp_path(char **path, char *first_arg)
{
	char	**tmp_path;
	int		i;

	i = 0;
	tmp_path = ft_calloc(sizeof(char *), (ft_tablen(path) + 1));
	if (tmp_path == NULL)
		return (NULL);
	while (path[i])
	{
		tmp_path[i] = ft_strjoin(path[i], first_arg);
		if (tmp_path[i] == NULL)
		{
			free_all(&tmp_path, i);
			return (NULL);
		}
		i++;
	}
	return (tmp_path);
}

static int	validate_assign_cmd(t_pipex *pipex_p, char **tmp_path, char *cmd)
{
	int	i;

	i = 0;
	while (tmp_path[i])
	{
		if (access(cmd, F_OK) == 0 && access(cmd, X_OK) == 0)
		{
			pipex_p->cmd_paths[pipex_p->cmd_count] = ft_strdup(cmd);
			if (pipex_p->cmd_paths[pipex_p->cmd_count] == NULL)
			{
				free_all(&(pipex_p->cmd_paths), pipex_p->cmd_count - 1);
				return (-1);
			}
			break ;
		}
		if (access(tmp_path[i], F_OK) == 0 && access(tmp_path[i], X_OK) == 0)
		{
			pipex_p->cmd_paths[pipex_p->cmd_count] = ft_strdup(tmp_path[i]);
			if (pipex_p->cmd_paths[pipex_p->cmd_count] == NULL)
			{
				free_all(&(pipex_p->cmd_paths), pipex_p->cmd_count - 1);
				return (-1);
			}
			break ;
		}
		i++;
	}
	return (0);
}

static int	parse_cmds_loop(t_pipex *pipex_p, char **ag, int ac, char **path)
{
	char	**tmp_path;
	int		j;
	char	**ag2;

	(void)ag;
	j = 2;
	while (j < ac - 1)
	{
		ag2 = ft_split(ag[j], ' ');
		if (ag2 == NULL)
		{
			free_tab(&pipex_p->cmd_paths);
			return (-1);
		}
		tmp_path = create_tmp_path(path, ag2[0]);
		if (tmp_path == NULL)
		{
			free_tab(&pipex_p->cmd_paths);
			free_tab(&ag2);
			return (-1);
		}
		if (validate_assign_cmd(pipex_p, tmp_path, ag2[0]) == -1)
		{
			free_tab(&tmp_path);
			free_tab(&ag2);
			return (-1);
		}
		free_tab(&ag2);
		free_tab(&tmp_path);
		pipex_p->cmd_count++;
		j++;
	}
	pipex_p->cmd_paths[pipex_p->cmd_count] = NULL;
	return (0);
}

int	parse_cmds(t_pipex *pipex_p, char **ag, int ac, char **envp)
{
	char	**path;

	if (init_paths(&path, envp) == -1)
		return (-1);
	pipex_p->cmd_paths = ft_calloc(sizeof(char *), (ac - 2));
	if (pipex_p->cmd_paths == NULL)
	{
		free_tab(&path);
		return (-1);
	}
	if (parse_cmds_loop(pipex_p, ag, ac, path) == -1)
	{
		free_tab(&path);
		return (-1);
	}
	free_tab(&path);
	return (0);
}

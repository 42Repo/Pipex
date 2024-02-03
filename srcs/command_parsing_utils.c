/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 21:41:25 by asuc              #+#    #+#             */
/*   Updated: 2024/02/03 03:33:22 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static char	**free_all_paths(char **cmd_args, int i)
{
	int	j;

	j = i + 2;
	while (i >= 0)
	{
		free(cmd_args[i]);
		i--;
	}
	while (cmd_args[j])
	{
		free(cmd_args[j]);
		j++;
	}
	free(cmd_args);
	return (NULL);
}

static char	**add_slash(char ***cmd_args)
{
	int		i;
	char	*tmp;

	i = 0;
	while ((*cmd_args)[i])
	{
		tmp = ft_strjoin((*cmd_args)[i], "/");
		free((*cmd_args)[i]);
		if (tmp == NULL)
		{
			free_all_paths(*cmd_args, i - 1);
			free(tmp);
			return (NULL);
		}
		(*cmd_args)[i] = ft_strdup(tmp);
		if ((*cmd_args)[i] == NULL)
		{
			free_all_paths(*cmd_args, i - 1);
			free(tmp);
			return (NULL);
		}
		free(tmp);
		i++;
	}
	return (*cmd_args);
}

static char	**find_path(char **envp)
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

int	init_paths(char ***path, char **envp)
{
	*path = find_path(envp);
	if (*path == NULL)
		return (-1);
	*path = add_slash(path);
	if (*path == NULL)
		return (-1);
	return (0);
}

int	fre_all(char ***cmd_args, int i)
{
	while (i >= 0)
	{
		free((*cmd_args)[i]);
		i--;
	}
	free((*cmd_args));
	return (-1);
}

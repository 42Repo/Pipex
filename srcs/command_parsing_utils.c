/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 21:41:25 by asuc              #+#    #+#             */
/*   Updated: 2024/01/26 21:55:23 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static char	**add_slash(char **cmd_args)
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
	*path = add_slash(*path);
	if (*path == NULL)
		return (-1);
	return (0);
}

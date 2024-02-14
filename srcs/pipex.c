/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 19:42:09 by asuc              #+#    #+#             */
/*   Updated: 2024/02/14 08:47:25 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static int	free_all(char ****cmd_args, int i)
{
	int	j;

	j = 0;
	while (i >= 0)
	{
		j = 0;
		while ((*cmd_args)[i][j])
		{
			free((*cmd_args)[i][j]);
			j++;
		}
		free((*cmd_args)[i]);
		i--;
	}
	free((*cmd_args));
	return (-1);
}

static int	parse_args(t_pipex *pipex_p, char **ag, int ac)
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
		{
			free_all(&(pipex_p->cmd_args), j - 1);
			return (-1);
		}
		i++;
		j++;
	}
	return (0);
}

static int	init_and_check(t_pipex *pipex_p, int ac, char **ag)
{
	init_pipex(pipex_p);
	pipex_p->pid = malloc(sizeof(pid_t) * (ac - 3));
	if (pipex_p->pid == NULL)
		return (0);
	if (check_args(pipex_p, ac, ag) == -1)
		return (clean_pipex(pipex_p, 1));
	return (0);
}

static int	parse_cmd_and_args(t_pipex *pipex_p, int ac, char **ag, char **envp)
{
	if (pipex_p->here_doc == false)
	{
		if (parse_cmds(pipex_p, ag, ac, envp) == -1)
			return (-1);
	}
	else
	{
		if (parse_cmds(pipex_p, ag + 1, ac - 1, envp) == -1)
			return (-1);
	}
	if (parse_args(pipex_p, ag, ac) == -1)
	{
		free_tab(&pipex_p->cmd_paths);
		return (-1);
	}
	return (0);
}

int	main(int ac, char **ag, char **envp)
{
	t_pipex	pipex_p;
	int		i;

	if (ac < 5)
		return (ft_putstr_fd("Error: not enough arguments\n", 2));
	if (init_and_check(&pipex_p, ac, ag) == -1)
	{
		unlink(pipex_p.random_name);
		return (-1);
	}
	if (parse_cmd_and_args(&pipex_p, ac, ag, envp) == -1)
		return (free_parsed_cmds(&pipex_p));
	i = 0;
	while (i < pipex_p.cmd_count)
	{
		if (exec_pipex(&pipex_p, i, envp) == -1)
			return (clean_pipex(&pipex_p, 1));
		i++;
	}
	i = 0;
	while (i < pipex_p.cmd_count)
		waitpid(pipex_p.pid[i++], NULL, 0);
	unlink(pipex_p.random_name);
	clean_pipex(&pipex_p, 1);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 21:13:29 by asuc              #+#    #+#             */
/*   Updated: 2024/02/03 03:35:19 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	close_all(t_pipex *pipex_p)
{
	if (pipex_p->in_fd > 0)
		close(pipex_p->in_fd);
	if (pipex_p->out_fd > 0)
		close(pipex_p->out_fd);
	return (0);
}

int	free_tab(char ***tab)
{
	int	i;

	i = 0;
	while ((*tab)[i] != NULL)
	{
		free((*tab)[i]);
		i++;
	}
	free(*tab);
	return (-1);
}

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
	close_all(pipex_p);
	if (pipex_p->limiter != NULL)
		free(pipex_p->limiter);
	if (mode == 0)
		return (-1);
	i = 0;
	while (i < pipex_p->cmd_count)
	{
		free_tab(&pipex_p->cmd_args[i]);
		i++;
	}
	free(pipex_p->cmd_args);
	return (-1);
}

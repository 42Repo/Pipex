/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 03:07:30 by asuc              #+#    #+#             */
/*   Updated: 2024/02/03 03:22:20 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

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

int	free_parsed_cmds(t_pipex *pipex_p)
{
	unlink(pipex_p->random_name);
	close_all(pipex_p);
	free(pipex_p->limiter);
	free(pipex_p->pid);
	return (-1);
}

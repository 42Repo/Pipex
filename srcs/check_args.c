/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 21:16:56 by asuc              #+#    #+#             */
/*   Updated: 2024/02/14 12:59:01 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static int	handle_doc(t_pipex *pipex_p, char *limiter_arg, char *outfile_arg)
{
	char	*limiter;

	pipex_p->here_doc = true;
	pipex_p->in_fd = open(pipex_p->random_name, O_CREAT | O_RDWR | O_TRUNC,
			0644);
	limiter = ft_strdup(limiter_arg);
	if (limiter == NULL)
		return (-1);
	pipex_p->limiter = ft_strjoin(limiter, "\n");
	free(limiter);
	if (pipex_p->limiter == NULL)
		return (-1);
	pipex_p->out_fd = open(outfile_arg, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (pipex_p->out_fd < 0)
		return (-1);
	else
		return (0);
}

static int	open_files(t_pipex *pipex_p, char *infile_arg, char *outfile_arg)
{
	pipex_p->in_fd = open(infile_arg, O_RDONLY);
	if (pipex_p->in_fd < 0)
		perror(infile_arg);
	pipex_p->out_fd = open(outfile_arg, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (pipex_p->out_fd < 0)
		perror(outfile_arg);
	if (pipex_p->out_fd < 0)
	{
		if (pipex_p->in_fd >= 0)
			close(pipex_p->in_fd);
		if (pipex_p->out_fd >= 0)
			close(pipex_p->out_fd);
		return (-1);
	}
	return (0);
}

int	check_args(t_pipex *pipex_p, int ac, char **ag)
{
	if (ac < 5)
		return (-1);
	if (ft_strncmp(ag[1], "here_doc", 8) == 0)
		return (handle_doc(pipex_p, ag[2], ag[ac - 1]));
	if (ft_strncmp(ag[1], "/dev/urandom", ft_strlen(ag[1])) == 0
		|| ft_strncmp(ag[1], "/dev/random", ft_strlen(ag[1])) == 0)
		pipex_p->is_invalid_infile = true;
	if (ft_strncmp(ag[1], "/dev/urandom", ft_strlen(ag[1])) == 0)
		pipex_p->random = file_urandom;
	if (ft_strncmp(ag[1], "/dev/random", ft_strlen(ag[1])) == 0)
		pipex_p->random = file_random;
	return (open_files(pipex_p, ag[1], ag[ac - 1]));
}

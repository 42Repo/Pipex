/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 21:26:31 by asuc              #+#    #+#             */
/*   Updated: 2024/02/14 11:19:19 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static int	test_open(t_pipex *pipex_p)
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

static int	read_line_and_compare(t_pipex *pipex, char **line)
{
	*line = get_next_line(0);
	if (*line == NULL)
	{
		ft_putstr_fd("pipex: warning: here-document delimited by end-of-file",
			2);
		return (1);
	}
	if (ft_strlen(*line) == ft_strlen(pipex->limiter) && ft_strncmp(*line,
			pipex->limiter, ft_strlen(pipex->limiter)) == 0)
		return (1);
	return (0);
}

static void	process_line(t_pipex *pipex, char **line, int *bol)
{
	if (ft_strlen(*line) >= 1 && (*line)[ft_strlen(*line) - 1] == '\n')
		*bol = 0;
	ft_putendl_fd(*line, pipex->in_fd);
}

int	here_doc(t_pipex *pipex)
{
	char	*line;
	int		bol;

	bol = 0;
	line = ft_strdup("");
	if (line == NULL)
		return (-1);
	while (1)
	{
		free(line);
		line = NULL;
		if (test_open(pipex) == -1)
			return (-1);
		if (bol == 0)
		{
			ft_printf("> ");
			bol = 1;
		}
		if (read_line_and_compare(pipex, &line))
			break ;
		if (line != NULL)
			process_line(pipex, &line, &bol);
	}
	free(line);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 19:44:39 by asuc              #+#    #+#             */
/*   Updated: 2024/01/23 17:14:12 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "Libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>

typedef enum e_bool
{
	false,
	true
}			t_bool;

typedef enum e_random
{
	file_urandom,
	file_random
}			t_random;

typedef struct s_pipex
{
	int			in_fd;
	int			out_fd;
	t_bool		here_doc;
	t_bool		is_invalid_infile;
	char		**cmd_paths;
	char		***cmd_args;
	int			cmd_count;
	char		*limiter;
	t_random	random;
	pid_t		*pid;
	char		random_name[20];
}			t_pipex;

int			*init_pipex(t_pipex *pipex);
int			check_args(t_pipex *pipex_p, int ac, char **ag);
int			close_all(t_pipex *pipex_p);
int			parse_cmds(t_pipex *pipex_p, char **ag, int ac, char **envp);
int			ft_tablen(char **tab);
void		free_tab(char ***tab);
char		**copy_tab(char **tab);
char		**find_path(char **envp);
char		**add_slash(char **cmd_args);
int			main(int ac, char **ag, char **envp);
int			exec_pipex(t_pipex *pipex_p, int i, char **envp);
void		invalid_infile(t_pipex *pipex_p, int pipefd[][2]);
int			here_doc(t_pipex *pipex);
int			test_open(t_pipex *pipex_p);
int			parse_args(t_pipex *pipex_p, char **ag, int ac);
int			clean_pipex(t_pipex *pipex_p, int mode);
int			check_exec_command(t_pipex *pipex_p, int i);
void		error_exit_fd(char *error_msg, int fd);
void		pipe_first_exec(t_pipex *pipex_p, int i, char **envp,
				int pipefd[][2]);
int			here_doc_exec(t_pipex *pipex_p, int i, char **envp,
				int pipefd[][2]);
void		invalid_file_exec(t_pipex *pipex_p, int i, char **envp,
				int pipefd[][2]);
int			error_pipex(char *error_msg);
void		pipe_exec(t_pipex *pipex_p, int i, char **envp, int pipefd[][2]);
void		child_exec(t_pipex *pipex_p, int i, char **envp, int pipefd[][2]);

#endif

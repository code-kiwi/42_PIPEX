/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 12:17:27 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/20 12:32:15 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <errno.h>
# include <string.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

# define FD_UNSET -2
# define FD_TREATED -3
# define ERROR_MESSAGE_ARGS "Usage: pipex infile cmd1 [...] cmdn outfile"
# define ERROR_MESSAGE_NULL_PTR "Error - Unexpected NULL pointer"
# define ERROR_MESSAGE_MALLOC "Error - A memory allocation failed"
# define PATH_STR_START "PATH="
# define PATH_STR_START_LEN 5
# define PATH_STR_SEPERATOR ":"
# define PATH_ABS_START "/"
# define PATH_ABS_START_LEN 1
# define PATH_REL_START "./"
# define PATH_REL_START_LEN 2

typedef struct s_pipex_data
{
	char	*program_name;
	char	**envp;
	t_list	*commands;
	t_list	*pids;
	char	**paths;
	int		fd_infile;
	int		fd_outfile;
}	t_pipex_data;

typedef struct s_command
{
	int		fd_in;
	int		fd_out;
	char	**args;
}	t_command;

void		init_pipex_data(t_pipex_data *data, char *prog_name, char **envp);
void		clean_pipex_data(t_pipex_data *data);

void		get_env_paths(t_pipex_data *data, char **envp);
void		handle_infile_outfile(t_pipex_data *data, int argc, char **argv);
void		get_commands(t_pipex_data *data, int argc, char **argv);
void		make_pipes(t_pipex_data *data);
void		exec_commands(t_pipex_data *data);
void		wait_pids(t_pipex_data *data);

t_command	*create_command(char **cmd_args);
void		delete_command(void *command);
void		display_command(t_command *cmd);

void		handle_error(t_pipex_data *data, \
				bool use_errno, char *error_msg, char *err_prec);
void		close_file(t_pipex_data *data, int fd);

#endif

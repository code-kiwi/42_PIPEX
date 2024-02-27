/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 12:17:27 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/27 19:24:06 by mhotting         ###   ########.fr       */
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

# define DEFAULT_PROG_NAME "./pipex"
# define PID_UNSET -2
# define FD_UNSET -2
# define FD_TREATED -3
# define ERROR_MESSAGE_ARGS \
	"Usage: pipex [infile | here_doc LIM] infile cmd1 [...] cmdn outfile"
# define ERROR_MESSAGE_NULL_PTR "Error - Unexpected NULL pointer"
# define ERROR_MESSAGE_MALLOC "Error - A memory allocation failed"
# define ERROR_MESSAGE_INTERNAL "Error - Internal error"
# define ERROR_MESSAGE_CMD "Command not found"
# define PATH_STR_START "PATH="
# define PATH_STR_START_LEN 5
# define PATH_STR_SEPERATOR ":"
# define PATH_ABS_START "/"
# define PATH_ABS_START_LEN 1
# define PATH_REL_START "./"
# define PATH_REL_START_LEN 2
# define HERE_DOC_STR "here_doc"

typedef struct s_pipex_data
{
	t_list	*commands;
	size_t	nb_commands;
	bool	here_doc_active;
	char	*program_name;
	char	**envp;
	char	**paths;
	int		fd_infile;
	int		fd_outfile;
	int		pipe_fds[2];
}	t_pipex_data;

typedef struct s_command
{
	char	**args;
	pid_t	pid;
	int		fd_in;
	int		fd_out;
}	t_command;

void		init_pipex_data(t_pipex_data *data, char *prog_name, char **envp);
void		close_pipex_pipe_fds(t_pipex_data *data);
void		clean_pipex_data(t_pipex_data *data);

bool		here_doc_read(char *limiter, int fd_to_write);
char		**get_env_paths(char **envp);
t_list		*get_commands(int argc, char **argv, bool here_doc_active);
int			handle_infile(char *infile);
int			handle_outfile(char *outfile, bool here_doc_active);
bool		set_cmd_fds(t_pipex_data *data, t_command *cmd, \
		bool is_first, bool is_last);
void		handle_command(t_pipex_data *data, t_command *cmd);
bool		wait_pids(t_list *commands);

t_command	*create_command(char **cmd_args);
void		delete_command(void *command);
void		display_command(t_command *cmd);
void		close_cmd_fds(t_command *cmd);

char		**pipex_split(char *str, char quote);

void		handle_error(t_pipex_data *data, \
				bool use_errno, char *error_msg, char *err_prec);
void		close_file(t_pipex_data *data, int fd);

#endif

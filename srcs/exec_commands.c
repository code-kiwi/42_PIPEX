/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 14:21:11 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/20 15:11:31 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 *	Returns the valid path (an allocated string) for a given command
 *	If the command is an absolute path, returns a duplicate str of the command
 *	Same thing happends for a relative path
 *	For other cases, checks if we can find a valid path to the command using
 *	the PATH from the program environment variables and returns it
 *	On error, NULL is returned
 */
static char	*get_command_path(t_pipex_data *data, char *given_cmd)
{
	size_t	i;
	char	**paths;
	char	*full_path;

	if (data == NULL || given_cmd == NULL)
		handle_error(data, false, ERROR_MESSAGE_NULL_PTR, NULL);
	if (strncmp(given_cmd, PATH_ABS_START, PATH_ABS_START_LEN) == 0)
		return (ft_strdup(given_cmd));
	if (strncmp(given_cmd, PATH_REL_START, PATH_REL_START_LEN) == 0)
		return (ft_strdup(given_cmd));
	paths = data->paths;
	i = 0;
	while (paths != NULL && paths[i] != NULL)
	{
		full_path = ft_strjoin(paths[i], given_cmd);
		if (full_path == NULL)
			handle_error(data, false, ERROR_MESSAGE_MALLOC, NULL);
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

/*
 *	Executes the given command
 *	STDIN and STDOUT are moved to the command fd_in and fd_out in order to read
 *	from and write into the appropriate fd
 *	This is and end point:
 *		- if execve() works, the executed program will take the lead
 *		- on error, the child process will exit properly, prompting an error msg
 */
static void	exec_command(t_pipex_data *data, t_command *cmd)
{
	int		returned;
	char	*command_path;

	if (data == NULL || cmd == NULL)
		handle_error(data, false, ERROR_MESSAGE_NULL_PTR, NULL);
	if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
		handle_error(data, true, NULL, NULL);
	if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
		handle_error(data, true, NULL, NULL);
	command_path = get_command_path(data, (cmd->args)[0]);
	if (command_path == NULL)
		handle_error(data, false, ERROR_MESSAGE_CMD, (cmd->args)[0]);
	returned = execve(command_path, cmd->args, data->envp);
	free(command_path);
	if (returned == -1)
		handle_error(data, false, strerror(errno), (cmd->args)[0]);
}

/*
 *	Creates a child process which will execute the given command
 *	The parent process will return the pid of the new child created
 *	The child process will never reach the end of the function
 *	On error, all the previous child process created will be waited before
 *	exiting properly the program
 */
static pid_t	cmd_fork(t_pipex_data *data, t_command *cmd)
{
	pid_t	pid;

	if (cmd == NULL)
		handle_error(data, false, ERROR_MESSAGE_NULL_PTR, NULL);
	pid = fork();
	if (pid == -1)
	{
		wait_pids(data);
		handle_error(data, true, NULL, NULL);
	}
	if (pid == 0)
		exec_command(data, cmd);
	return (pid);
}

/*
 *	Goes through all the commands of the given data and creates a child process
 *	for each of them, then registers the pid of the child process into the
 *	command structure
 *	After having created the child process, the parent process closes the
 *	command file descriptors because they are useless for the parent, and only
 *	used by the created child
 *	If a command does not have a valid fd, the command will not be executed
 */
void	exec_commands(t_pipex_data *data)
{
	t_list		*curr_command;
	t_command	*cmd;
	pid_t		pid;

	if (data == NULL || data->commands == NULL)
		handle_error(data, false, ERROR_MESSAGE_NULL_PTR, NULL);
	curr_command = data->commands;
	while (curr_command != NULL)
	{
		cmd = (t_command *)(curr_command->content);
		if (
			cmd->fd_in != -1 && cmd->fd_out != -1
			&& cmd->fd_in != FD_UNSET && cmd->fd_out != FD_UNSET
		)
		{
			pid = cmd_fork(data, cmd);
			cmd->pid = pid;
			close_cmd_fds(cmd);
		}
		curr_command = curr_command->next;
	}
}

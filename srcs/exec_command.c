/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 19:18:22 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/27 17:26:26 by mhotting         ###   ########.fr       */
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
static char	*get_command_path(char **paths, char *given_cmd)
{
	size_t	i;
	char	*full_path;

	if (paths == NULL || given_cmd == NULL)
		return (NULL);
	if (strncmp(given_cmd, PATH_ABS_START, PATH_ABS_START_LEN) == 0)
		return (ft_strdup(given_cmd));
	if (strncmp(given_cmd, PATH_REL_START, PATH_REL_START_LEN) == 0)
		return (ft_strdup(given_cmd));
	i = 0;
	while (paths[i] != NULL)
	{
		full_path = ft_strjoin(paths[i], given_cmd);
		if (full_path == NULL)
			return (NULL);
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
 *	Unused file descriptors are then closed
 *	This function is and end point:
 *		- if execve() works, the executed program will take the lead
 *		- on error, the child process will exit properly, prompting an error msg
 */
static void	exec_command(t_pipex_data *data, t_command *cmd)
{
	char	*command_path;

	if (data == NULL || cmd == NULL)
		handle_error(data, false, ERROR_MESSAGE_NULL_PTR, NULL);
	if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
		handle_error(data, true, NULL, NULL);
	close_file(NULL, cmd->fd_in);
	cmd->fd_in = FD_TREATED;
	if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
		handle_error(data, true, NULL, NULL);
	close_file(NULL, cmd->fd_out);
	cmd->fd_out = FD_TREATED;
	close_pipex_pipe_fds(data);
	command_path = get_command_path(data->paths, (cmd->args)[0]);
	if (command_path == NULL)
	{
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		handle_error(data, false, ERROR_MESSAGE_CMD, (cmd->args)[0]);
	}
	execve(command_path, cmd->args, data->envp);
	free(command_path);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	handle_error(data, true, NULL, NULL);
}

/*
 *	Creates a child process which will execute the given command
 *	The parent process will return the pid of the new child created
 *	The child process will never reach the end of the function
 *	On error, all the previous child process created will be waited before
 *	exiting properly the program
 */
void	handle_command(t_pipex_data *data, t_command *cmd)
{
	pid_t		pid;

	if (data == NULL || cmd == NULL)
		handle_error(data, false, ERROR_MESSAGE_NULL_PTR, NULL);
	pid = fork();
	if (pid == -1)
	{
		wait_pids(data->commands);
		handle_error(data, true, NULL, NULL);
	}
	if (pid == 0)
		exec_command(data, cmd);
	cmd->pid = pid;
}

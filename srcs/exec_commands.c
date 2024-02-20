/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 14:21:11 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/20 13:07:01 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
		handle_error(data, false, "Command not found: ", (cmd->args)[0]);
	returned = execve(command_path, cmd->args, data->envp);
	if (returned == -1)
		handle_error(data, true, NULL, NULL);
	free(command_path);
}

static pid_t	cmd_fork(t_pipex_data *data, t_command *cmd)
{
	pid_t	pid;

	if (cmd == NULL)
		handle_error(data, false, ERROR_MESSAGE_NULL_PTR, NULL);
	pid = fork();
	if (pid == -1)
		handle_error(data, true, NULL, NULL);
	if (pid == 0)
		exec_command(data, cmd);
	return (pid);
}

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
			// ADD PID TO PIDS
		}
		curr_command = curr_command->next;
	}
}

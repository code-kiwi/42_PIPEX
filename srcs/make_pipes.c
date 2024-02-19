/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 14:18:28 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/19 17:06:30 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 *	Goes through all commands registered into data->commands and attributes
 *	them their file descriptors (using pipes)
 *	The first command will have the data->fd_infile as input fd
 *	The last command will have the data->fd_outfile as output fd
 *	data->infile and data->outfile will be given FD_TREATED values
 *	In case of error, the program is properly left (memory freed, fd closed)
 */
void	make_pipes(t_pipex_data *data)
{
	t_list		*curr_command;
	t_command	*cmd;
	int			fd[2];

	if (data == NULL || data->commands == NULL)
		handle_error(data, false, ERROR_MESSAGE_ARGS);
	curr_command = data->commands;
	cmd = (t_command *)(curr_command->content);
	cmd->fd_in = data->fd_infile;
	data->fd_infile = FD_TREATED;
	while (curr_command->next != NULL)
	{
		if (pipe(fd) == -1)
			handle_error(data, true, NULL);
		cmd->fd_out = fd[1];
		curr_command = curr_command->next;
		cmd = (t_command *)(curr_command->content);
		cmd->fd_in = fd[0];
	}
	cmd->fd_out = data->fd_outfile;
	data->fd_outfile = FD_TREATED;
}

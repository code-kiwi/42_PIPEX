/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_cmd_fds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 20:07:19 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/26 14:35:05 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 *	Attributes a command its file descriptors
 *	If the command is the first of the list, its fd_in is set to data fd_infile
 *	If the command is the last of the list, its ft_out is set to data fd_outfile
 *	Other commands have their fds set to pipes input and output, saving
 *	opened fds into data pipe_fds in order to keep a trace of pipes unused ends
 */
bool	set_cmd_fds(
	t_pipex_data *data, t_command *cmd, bool is_first, bool is_last
)
{
	if (is_first)
	{
		cmd->fd_in = data->fd_infile;
		data->fd_infile = FD_TREATED;
	}
	else
	{
		cmd->fd_in = data->pipe_fds[0];
		data->pipe_fds[0] = FD_UNSET;
	}
	if (is_last)
	{
		cmd->fd_out = data->fd_outfile;
		data->fd_outfile = FD_TREATED;
		return (true);
	}
	if (pipe(data->pipe_fds) == -1)
	{
		close_file(data, cmd->fd_in);
		cmd->fd_in = FD_UNSET;
		return (false);
	}
	cmd->fd_out = data->pipe_fds[1];
	data->pipe_fds[1] = FD_UNSET;
	return (true);
}

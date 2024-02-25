/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_data_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:15:44 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/25 21:01:59 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 *	Sets the defaults values of a t_pipex_data element
 */
void	init_pipex_data(t_pipex_data *data, char *program_name, char **envp)
{
	data->program_name = program_name;
	data->envp = envp;
	data->commands = NULL;
	data->paths = NULL;
	data->fd_infile = FD_UNSET;
	data->fd_outfile = FD_UNSET;
	data->pipe_fds[0] = FD_UNSET;
	data->pipe_fds[1] = FD_UNSET;
}

/*
 *	Cleans all the memory used by a t_pipex_data element
 *	Also closes the file descriptors used by the program
 */
void	clean_pipex_data(t_pipex_data *data)
{
	if (data == NULL)
		handle_error(data, false, ERROR_MESSAGE_NULL_PTR, NULL);
	if (data->paths != NULL)
		ft_free_str_array(&(data->paths));
	if (
		data->fd_infile != FD_TREATED && data->fd_infile != -1
		&& data->fd_infile != FD_UNSET
	)
		close_file(data, data->fd_infile);
	if (
		data->fd_outfile != FD_TREATED && data->fd_outfile != -1
		&& data->fd_outfile != FD_UNSET
	)
		close_file(data, data->fd_outfile);
	if (data->commands != NULL)
	{
		wait_pids(data->commands);
		ft_lstclear(&(data->commands), &delete_command);
	}
	if ((data->pipe_fds)[0] != -1 && (data->pipe_fds)[0] != FD_UNSET)
		close_file(data, (data->pipe_fds)[0]);
	if ((data->pipe_fds)[1] != -1 && (data->pipe_fds)[1] != FD_UNSET)
		close_file(data, (data->pipe_fds)[1]);
}

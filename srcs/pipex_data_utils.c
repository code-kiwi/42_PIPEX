/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_data_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:15:44 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/27 17:44:12 by mhotting         ###   ########.fr       */
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
	data->here_doc_active = false;
}

/*
 *	Given a pipex data element, closes it pipe_fds if they are opened
 *	When the fds are closed, their value is set to FD_UNSET
 */
void	close_pipex_pipe_fds(t_pipex_data *data)
{
	int	*fds;

	if (data == NULL)
		handle_error(data, false, ERROR_MESSAGE_NULL_PTR, NULL);
	fds = (int *)(data->pipe_fds);
	if (fds[0] != -1 && fds[0] != FD_UNSET && fds[0] != FD_TREATED)
	{
		close_file(data, fds[0]);
		fds[0] = FD_UNSET;
	}
	if (fds[1] != -1 && fds[1] != FD_UNSET && fds[1] != FD_TREATED)
	{
		close_file(data, fds[1]);
		fds[1] = FD_UNSET;
	}
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
		ft_lstclear(&(data->commands), &delete_command);
	close_pipex_pipe_fds(data);
}

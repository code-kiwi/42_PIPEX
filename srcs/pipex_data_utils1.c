/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_data_utils1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 13:35:54 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/19 11:46:54 by mhotting         ###   ########.fr       */
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
	data->pids = NULL;
	data->paths = NULL;
	data->fd_infile = -1;
	data->fd_outfile = -1;
}

/*
 *	Cleans all the memory used by a t_pipex_data element
 *	Also closes the file descriptors used by the program
 */
void	clean_pipex_data(t_pipex_data *data)
{
	if (data == NULL)
		return (handle_error(data, false, ERROR_MESSAGE_NULL_PTR));
	if (data->paths != NULL)
		ft_free_str_array(&(data->paths));
	if (data->fd_infile != -1)
		close_file(data, data->fd_infile);
	if (data->fd_outfile != -1)
		close_file(data, data->fd_outfile);
	if (data->commands != NULL)
		ft_lstclear(&(data->commands), &delete_command);
}

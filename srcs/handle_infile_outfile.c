/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_infile_outfile.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 18:12:25 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/19 17:03:10 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 *	Opens infile and outfile given as program parameter in order to associate
 *	a file descriptor to them and register them into the project main struct
 *	In case of error, the appropriate error message is printed but the
 *	execution keeps going (main structure fds are set to FD_UNSET by default
 *	and will keep this value when an error occurs)
 */
void	handle_infile_outfile(t_pipex_data *data, int argc, char **argv)
{
	int	fd;

	if (data == NULL || argv == NULL || argc < 1)
		handle_error(data, false, ERROR_MESSAGE_NULL_PTR, NULL);
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		ft_dprintf(STDERR_FILENO, "%s: Error - %s: %s\n", data->program_name, \
				strerror(errno), argv[1]);
	}
	else
		data->fd_infile = fd;
	fd = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd == -1)
	{
		ft_dprintf(STDERR_FILENO, "%s: Error - %s: %s\n", data->program_name, \
				strerror(errno), argv[argc - 1]);
	}
	else
		data->fd_outfile = fd;
}

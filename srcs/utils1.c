/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 11:42:46 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/18 19:03:37 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 *	Handles error for the pipex program
 *	Cleans data if it is not NULL, can use errno when use_errno is set and
 *	errno value is usable and exits the program after having prompted the
 *	appropriate error message
 */
void	handle_error(t_pipex_data *data, bool use_errno, char *error_message)
{
	if (data != NULL)
		clean_pipex_data(data);
	if (use_errno)
	{
		if (errno != 0)
			ft_dprintf(STDERR_FILENO, "Error: %s\n", strerror(errno));
		else
			ft_dprintf(STDERR_FILENO, "Error: Unknown error occured\n");
	}
	else if (error_message != NULL)
		ft_dprintf(STDERR_FILENO, "Error: %s\n", error_message);
	else
		ft_dprintf(STDERR_FILENO, "Error: Unknown error occured\n");
	exit(EXIT_FAILURE);
}

/*
 *	Closes the given file descriptor
 *	In case of error, the appropriate message is printed
 */
void	close_file(t_pipex_data *data, int fd)
{
	if (close(fd) == -1)
	{
		if (data != NULL)
		{
			ft_dprintf(STDERR_FILENO, "%s: Error - %s\n", data->program_name, \
					strerror(errno));
		}
		else
			ft_dprintf(STDERR_FILENO, "Error - %s\n", strerror(errno));
	}
}

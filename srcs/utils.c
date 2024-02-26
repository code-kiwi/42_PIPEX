/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:18:10 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/26 14:02:27 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 *	Handles error for the pipex program
 *	Cleans data if it is not NULL, can use errno when use_errno is set and
 *	errno value is usable and exits the program after having prompted the
 *	appropriate error message
 */
void	handle_error(
	t_pipex_data *data, bool use_errno,
	char *err_message, char *err_prec
)
{
	char	*prog;

	if (data != NULL)
		prog = data->program_name;
	else
		prog = DEFAULT_PROG_NAME;
	if (use_errno)
	{
		if (errno != 0)
			ft_dprintf(STDERR_FILENO, "%s: %s\n", prog, strerror(errno));
		else
			ft_dprintf(STDERR_FILENO, "%s: Unknown error\n", prog);
	}
	else if (err_message != NULL && err_prec != NULL)
		ft_dprintf(STDERR_FILENO, "%s: %s: %s\n", prog, err_message, err_prec);
	else if (err_message != NULL)
		ft_dprintf(STDERR_FILENO, "%s: %s\n", prog, err_message);
	else
		ft_dprintf(STDERR_FILENO, "%s: Unknown error\n", prog);
	if (data != NULL)
		clean_pipex_data(data);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_infile_outfile.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 14:10:29 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/27 19:27:12 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 *	Reads lines from STDIN_FILENO and writes them into the given ft_to_write
 *	When limiter is encountered, the reading process stops
 *	Returns true on success, false on error
 */
bool	here_doc_read(char *limiter, int fd_to_write)
{
	char	*cur_line;

	if (limiter == NULL || fd_to_write < 0)
		return (false);
	while (true)
	{
		if (ft_printf("> ") == -1)
			return (false);
		cur_line = get_next_line(STDIN_FILENO);
		if (cur_line == NULL)
			continue ;
		if (ft_strncmp(cur_line, limiter, ft_strlen(cur_line) - 1) == 0)
			break ;
		if (ft_dprintf(fd_to_write, "%s", cur_line) == -1)
		{
			free(cur_line);
			get_next_line(-1);
			return (false);
		}
		free(cur_line);
	}
	get_next_line(-1);
	free(cur_line);
	return (true);
}

/*
 *	Opens the given file with read only mode
 *	On error, -1 is returned (open error or invalid given file error)
 */
int	handle_infile(char *infile)
{
	int	fd;

	if (infile == NULL)
		return (-1);
	fd = open(infile, O_RDONLY);
	return (fd);
}

/*
 *	Opens the given file in order to create it if it does not exist
 *	if here_doc is not active, writes to the file by erasing its content
 *	else, writes to the file by appending content to it
 *	The created file has 644 rights (RW-R--R--)
 *	On error, -1 is returned
 */
int	handle_outfile(char *outfile, bool here_doc_active)
{
	int	fd;

	if (outfile == NULL)
		return (-1);
	if (here_doc_active)
		fd = open(outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else
		fd = open(outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	return (fd);
}

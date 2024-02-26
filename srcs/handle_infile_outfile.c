/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_infile_outfile.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 14:10:29 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/25 15:03:15 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
 *	Opens the given file in order to create it if it does not exist, write
 *	to the file by erasing its content
 *	The created file has 644 rights (RW-R--R--)
 *	On error, -1 is returned (open error or invalid given file error)
 */
int	handle_outfile(char *outfile)
{
	int	fd;

	if (outfile == NULL)
		return (-1);
	fd = open(outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	return (fd);
}

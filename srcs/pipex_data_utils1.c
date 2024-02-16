/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_data_utils1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 13:35:54 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/16 13:37:36 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_pipex_data(t_pipex_data *data)
{
	data->commands = NULL;
	data->pids = NULL;
	data->paths = NULL;
	data->fd_infile = -1;
	data->fd_outfile = -1;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 12:16:35 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/19 12:52:47 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex_data	data;

	if (argc < 5)
		handle_error(NULL, false, ERROR_MESSAGE_ARGS);
	init_pipex_data(&data, argv[0], envp);
	get_env_paths(&data, envp);
	handle_infile_outfile(&data, argc, argv);
	get_commands(&data, argc, argv);
	make_pipes(&data);
	exec_commands(&data);
	wait_pids(&data);
	clean_pipex_data(&data);
	return (0);
}

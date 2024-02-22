/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 15:55:03 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/22 16:55:44 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	parse_args(t_pipex_data *data, int argc, char **argv, char **envp)
{
	if (data == NULL || argv == NULL || envp == NULL)
		handle_error(data, false, ERROR_MESSAGE_NULL_PTR, NULL);
	data->paths = get_env_paths(envp);
	if (data->paths == NULL)
		handle_error(data, true, NULL, NULL);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex_data	data;

	if (argc < 5)
		handle_error(NULL, false, ERROR_MESSAGE_ARGS, NULL);
	init_pipex_data(&data, argv[0], envp);
	parse_args(&data, argc, argv, envp);
	clean_pipex_data(&data);
	return (0);
}

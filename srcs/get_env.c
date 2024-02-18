/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 14:17:26 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/18 13:34:46 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 *	Extracts the different paths from the given string
 *	Splits the PATH env. variable into pieces (seperated by ':')
 */
static void	extract_env(t_pipex_data *data, char *env_str)
{
	char	*sub_str;
	char	**res;

	if (
		data == NULL || env_str == NULL
		|| ft_strncmp(env_str, PATH_STR_START, PATH_STR_START_LEN) != 0
	)
		handle_error(data, false, ERROR_MESSAGE_NULL_PTR);
	sub_str = ft_substr(env_str, PATH_STR_START_LEN, ft_strlen(env_str));
	if (sub_str == NULL)
		handle_error(data, false, ERROR_MESSAGE_MALLOC);
	res = ft_split(sub_str, PATH_STR_SEPERATOR);
	free(sub_str);
	if (res == NULL)
		handle_error(data, false, ERROR_MESSAGE_MALLOC);
	data->paths = res;
}

/*
 *	Searches for the PATH in the environment variables
 *	When found, set data->paths to an array containing all the paths
 *	Exits the program in case of error
 */
void	get_env_paths(t_pipex_data *data, char **envp)
{
	size_t	i;

	if (data == NULL || envp == NULL)
		handle_error(data, false, ERROR_MESSAGE_NULL_PTR);
	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], PATH_STR_START, PATH_STR_START_LEN) == 0)
			extract_env(data, envp[i]);
		i++;
	}
}

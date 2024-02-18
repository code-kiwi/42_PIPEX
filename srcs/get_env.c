/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 14:17:26 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/18 17:39:26 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 *	Splits the given str into tokens
 *	Each token is duplicated, adding a '/' character at the end in order
 *	to have a valid path
 *	In case of error, false is returned, else true is returned
 */
static bool	split_env(char *path_str, char **res, size_t nb_paths)
{
	size_t	i;
	char	*path_token;
	char	*temp;

	if (path_str == NULL || res == NULL)
		return (false);
	i = 0;
	path_token = ft_strtok(path_str, PATH_STR_SEPERATOR);
	while (i < nb_paths && path_token != NULL)
	{
		temp = ft_append_chars(path_token, '/', ft_strlen(path_token) + 1);
		if (temp == NULL)
			return (false);
		res[i] = temp;
		path_token = ft_strtok(NULL, PATH_STR_SEPERATOR);
		i++;
	}
	return (true);
}

/*
 *	Puts the different paths from the given string into data->paths
 *	An array of strings is allocated in order to store the paths (NULL
 *	terminated array)
 *	In case of error, the memory is freed and the program is left
 */
static void	extract_env(t_pipex_data *data, char *path_str)
{
	char	*sub_str;
	size_t	nb_paths;

	if (
		data == NULL || path_str == NULL
		|| ft_strncmp(path_str, PATH_STR_START, PATH_STR_START_LEN) != 0
	)
		handle_error(data, false, ERROR_MESSAGE_NULL_PTR);
	sub_str = ft_substr(path_str, PATH_STR_START_LEN, ft_strlen(path_str));
	if (sub_str == NULL)
		handle_error(data, false, ERROR_MESSAGE_MALLOC);
	nb_paths = ft_count_words(sub_str, PATH_STR_SEPERATOR);
	data->paths = (char **) ft_calloc((nb_paths + 1), sizeof(char *));
	if (data->paths == NULL)
	{
		free(sub_str);
		handle_error(data, false, ERROR_MESSAGE_MALLOC);
	}
	if (!split_env(sub_str, data->paths, nb_paths))
	{
		free(sub_str);
		ft_free_str_array(&(data->paths));
		handle_error(data, false, ERROR_MESSAGE_MALLOC);
	}
	free(sub_str);
}

/*
 *	Searches for the PATH in the environment variables
 *	When found, set data->paths to an array containing all the paths
 *	Exits the program in case of error, using the function to release resources
 *	properly
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

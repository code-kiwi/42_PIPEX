/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_paths.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:44:10 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/22 16:52:58 by mhotting         ###   ########.fr       */
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
 *	Puts the different paths from the given string into a NULL terminated array
 *	This array is allocated
 *	The given path_str is expected to start with "PATH=" or an undefined
 *	result would be returned by the function
 *	Returns NULL on error
 */
static char	**extract_env(char *path_str)
{
	char	*sub_str;
	size_t	nb_paths;
	char	**res;

	if (path_str == NULL)
		return (NULL);
	sub_str = ft_substr(path_str, PATH_STR_START_LEN, ft_strlen(path_str));
	if (sub_str == NULL)
		return (NULL);
	nb_paths = ft_count_words(sub_str, PATH_STR_SEPERATOR);
	res = (char **) ft_calloc((nb_paths + 1), sizeof(char *));
	if (res == NULL)
	{
		free(sub_str);
		return (NULL);
	}
	if (!split_env(sub_str, res, nb_paths))
	{
		free(sub_str);
		ft_free_str_array(&res);
		return (NULL);
	}
	free(sub_str);
	return (res);
}

/*
 *	Searches for the PATH in the environment variables
 *	When found, splits it into a null terminated array containing all the paths
 *	Returns NULL on error
 */
char	**get_env_paths(char **envp)
{
	size_t	i;

	if (envp == NULL)
		return (NULL);
	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], PATH_STR_START, PATH_STR_START_LEN) == 0)
			return (extract_env(envp[i]));
		i++;
	}
	return (NULL);
}

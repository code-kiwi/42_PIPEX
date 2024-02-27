/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:58:51 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/27 11:39:35 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 *	Given a NULL terminated array of args (a command and its paremeters)
 *	saves it into a command structure and puts this structure at the end of
 *	the commands given list
 *	Returns true on success, false on error
 */
static bool	add_cmd_to_data(t_list **commands, char **cmd_args)
{
	t_command	*cmd;
	t_list		*node;

	if (commands == NULL || cmd_args == NULL)
		return (false);
	cmd = create_command(cmd_args);
	if (cmd == NULL)
		return (false);
	node = ft_lstnew((void *) cmd);
	if (node == NULL)
	{
		free(cmd);
		return (false);
	}
	ft_lstadd_back(commands, node);
	return (true);
}

/*
 *	Given a command string, splits it into valid chunks and returns a NULL
 *	terminated array containing all the chunks
 *	Handles cases with quotes or double quotes into the input command
 *	Returns NULL on error
 *	On success, a pointer to the resulting array is returned
 */
static char	**get_cmd_args(char *cmd)
{
	char	*str;
	char	*str2;
	char	quote;

	str = NULL;
	str2 = NULL;
	if (cmd == NULL)
		return (NULL);
	if (ft_strchr(cmd, '\'') != NULL)
		quote = '\'';
	else
		quote = '"';
	str = ft_strchr(cmd, quote);
	if (str != NULL)
		str2 = ft_strchr(str + 1, quote);
	if (str != NULL && str2 != NULL)
		return (pipex_split(cmd, quote));
	else
		return (ft_split(cmd, " "));
}

/*
 *	Extracts all the commands from argv in order to build a t_list of t_commands
 *	Returns NULL on error
 *	On success, returns a pointer to the created list
 */
t_list	*get_commands(int argc, char **argv)
{
	size_t	i;
	char	**cmd_args;
	t_list	*commands;

	if (argv == NULL)
		return (NULL);
	commands = NULL;
	i = 2;
	while ((int) i < argc - 1)
	{
		cmd_args = get_cmd_args(argv[i]);
		if (cmd_args == NULL)
		{
			ft_lstclear(&commands, &delete_command);
			return (NULL);
		}
		if (!add_cmd_to_data(&commands, cmd_args))
		{
			ft_lstclear(&commands, &delete_command);
			ft_free_str_array(&cmd_args);
			return (NULL);
		}
		i++;
	}
	return (commands);
}

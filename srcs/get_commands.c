/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 14:16:29 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/19 12:55:32 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 *	Given a NULL terminated array of args (a command and its paremeters)
 *	saves it into a command structure and puts this structure at the end of
 *	data->commands, which is a linked list
 *	Returns true on success, false on error (cleans its resources in this case)
 */
static bool	add_cmd_to_data(t_pipex_data *data, char **cmd_args)
{
	t_command	*cmd;
	t_list		*node;

	if (data == NULL || cmd_args == NULL)
		return (false);
	cmd = create_command(cmd_args);
	if (cmd == NULL)
		return (false);
	node = ft_lstnew((void *) cmd);
	if (node == NULL)
	{
		delete_command((void *) cmd);
		return (false);
	}
	ft_lstadd_back(&(data->commands), node);
	return (true);
}

/*
 *	Given a command string, splits it into valid chunks and returns a NULL
 *	terminated array containing all the chunks
 *	In case of error, NULL is returned
 *	On success, a pointer to the resulting array is returned
 */
static char	**get_cmd_args(char *cmd)
{
	if (cmd == NULL)
		return (NULL);
	return (ft_split(cmd, " "));
}

/*
 *	Extracts all the commands from argv in order to register them into
 *	the commands of the t_pipex_data given structure
 *	In case of error, the program is left and the resources are properly
 *	removed (memory freed and fd closed
 */
void	get_commands(t_pipex_data *data, int argc, char **argv)
{
	size_t	i;
	char	**cmd_args;

	if (data == NULL || argv == NULL)
		handle_error(data, false, ERROR_MESSAGE_NULL_PTR);
	i = 2;
	while ((int) i < argc - 1)
	{
		cmd_args = get_cmd_args(argv[i]);
		if (cmd_args == NULL)
			handle_error(data, false, ERROR_MESSAGE_MALLOC);
		if (!add_cmd_to_data(data, cmd_args))
			handle_error(data, false, ERROR_MESSAGE_MALLOC);
		i++;
	}
}

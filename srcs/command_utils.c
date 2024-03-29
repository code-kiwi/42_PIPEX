/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:27:42 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/26 13:06:44 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 *	Creates a new command and returns a pointer to the freshly allocated command
 *	By default, fd_in and fd_out are set to -1
 *	In case of error, NULL is returned
 */
t_command	*create_command(char **cmd_args)
{
	t_command	*cmd;

	if (cmd_args == NULL)
		return (NULL);
	cmd = (t_command *) malloc(sizeof(t_command));
	if (cmd == NULL)
		return (cmd);
	cmd->fd_in = FD_UNSET;
	cmd->fd_out = FD_UNSET;
	cmd->args = cmd_args;
	cmd->pid = PID_UNSET;
	return (cmd);
}

/*
 *	Deletes a command by closing its file descriptors and freeing its args
 *	Frees the pointer itself if not NULL
 */
void	delete_command(void *command)
{
	t_command	*cmd;

	if (command == NULL)
		return ;
	cmd = (t_command *) command;
	close_cmd_fds(cmd);
	if (cmd->args != NULL)
		ft_free_str_array(&(cmd->args));
	free(cmd);
}

/*
 *	Displays the content of a given command
 */
void	display_command(t_command *cmd)
{
	size_t	i;
	char	**args;

	if (cmd == NULL)
	{
		ft_printf("The given command is NULL\n");
		ft_printf("--------------------\n");
		return ;
	}
	args = cmd->args;
	if (cmd->args != NULL && args[0] != NULL)
	{
		i = 0;
		while (args[i] != NULL)
		{
			ft_printf("INDEX %d: \"%s\"\n", (int) i, args[i]);
			i++;
		}
	}
	else
		ft_printf("The given command has no args\n");
	ft_printf("fd_in: %d - fd_out: %d\n", cmd->fd_in, cmd->fd_out);
	ft_printf("PID: %d\n", cmd->pid);
	ft_printf("--------------------\n");
}

/*
 *	Closes both fd_in and fd_out of the given command
 *	If a file descriptor's value is -1, FD_UNSET or FD_TREATED, then close_file
 *	is not called
 *	On error, nothing happens (closing file errors are managed by close_file()
 */
void	close_cmd_fds(t_command *cmd)
{
	if (cmd == NULL)
		return ;
	if (cmd->fd_in != -1 && cmd->fd_in != FD_UNSET && cmd->fd_in != FD_TREATED)
	{
		close_file(NULL, cmd->fd_in);
		cmd->fd_in = FD_UNSET;
	}
	if (
		cmd->fd_out != -1 && cmd->fd_out != FD_UNSET
		&& cmd->fd_out != FD_TREATED
	)
	{
		close_file(NULL, cmd->fd_out);
		cmd->fd_out = FD_UNSET;
	}
}

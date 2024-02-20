/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_pids.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 14:15:18 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/20 14:32:09 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 *	Goes through all the commands registered in the given data element and
 *	waits for every child process to be done if the command pid is set
 *	If the input data is invalid, the program exits properly
 *	If waitpid fails, an error message is prompted but the execution does not
 *	stop
 */
void	wait_pids(t_pipex_data *data)
{
	t_list		*curr_cmd;
	t_command	*cmd;

	if (data == NULL)
		handle_error(data, false, ERROR_MESSAGE_NULL_PTR, NULL);
	curr_cmd = data->commands;
	while (curr_cmd != NULL)
	{
		cmd = (t_command *)(curr_cmd->content);
		if (cmd->pid != PID_UNSET && cmd->pid > 0)
		{
			if (waitpid(cmd->pid, NULL, 0) == -1)
				ft_dprintf(STDERR_FILENO, "%s\n", ERROR_MESSAGE_INTERNAL);
			cmd->pid = PID_UNSET;
		}
		curr_cmd = curr_cmd->next;
	}
}

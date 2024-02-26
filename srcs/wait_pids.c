/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_pids.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:24:31 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/26 14:27:26 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 *	Goes through all the commands registered in the given command list and
 *	waits for every child process to be done (if the command pid is set)
 *	Returns true if no error has been detected during the process
 *	On error, false is returned. An error can be:
 *		- Bad input
 *		- If waitpid fails (then a message if prompted);
 *		- If a child process ended unnormally;
 *	On waitpid error or child process error, the waitpid loop is not stopped
 */
bool	wait_pids(t_list *commands)
{
	t_command	*cmd;
	int			status;
	bool		no_error_detected;

	if (commands == NULL)
		return (false);
	no_error_detected = true;
	while (commands != NULL)
	{
		cmd = (t_command *)(commands->content);
		if (cmd->pid != PID_UNSET && cmd->pid > 0)
		{
			if (waitpid(cmd->pid, &status, 0) == -1)
				no_error_detected = false;
			if (!WIFEXITED(status) && WEXITSTATUS(status))
				no_error_detected = false;
			cmd->pid = PID_UNSET;
		}
		commands = commands->next;
	}
	return (no_error_detected);
}

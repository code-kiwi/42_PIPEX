/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_pids.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:24:31 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/28 23:09:08 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 *	Goes through all the commands registered in the given command list and
 *	waits for every child process to be done (if the command pid is set)
 *	Returns 0 if no error has been detected during the process
 *	On error, a status is returned. An error can be:
 *		- Bad input (status is then EXIT_FAILURE)
 *		- If waitpid fails (status is then EXIT_FAILURE);
 *		- If a child process ended unnormally (gets the process status);
 *	On waitpid error or child process error, the waitpid loop is not stopped
 */
int	wait_pids(t_list *commands)
{
	t_command	*cmd;
	int			status;
	int			ret;

	if (commands == NULL)
		return (EXIT_FAILURE);
	ret = 0;
	while (commands != NULL)
	{
		cmd = (t_command *)(commands->content);
		if (cmd->pid != PID_UNSET && cmd->pid > 0)
		{
			if (waitpid(cmd->pid, &status, 0) == -1)
				ret = EXIT_FAILURE;
			if (ret == 0 && commands->next == NULL && !WIFEXITED(status))
				ret = WEXITSTATUS(status);
			else if (ret == 0 && commands->next == NULL && WEXITSTATUS(status))
				ret = WEXITSTATUS(status);
			cmd->pid = PID_UNSET;
		}
		commands = commands->next;
	}
	return (ret);
}

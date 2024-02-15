/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fake_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 15:15:44 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/15 15:56:33 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int	main(int argc, char **argv)
{
	int	fd[2];

	if (pipe(fd) == -1)
		return (1);
	
	int	pid1 = fork();
	if (pid1 == -1)
		return (2);

	if (pid1 == 0)
	{
		// Child process 1 (for ping) => nothing after because the process will be replacede by the process from execlp() function
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]); // can be closed because STDOUT_FILENO points to the pipe now
		execlp("ping", "ping", "-c", "5", "google.com", NULL);
	}

	int pid2 = fork();
	if (pid2 == -1)
		return(2);
	if (pid2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		execlp("grep", "grep", "rtt", NULL);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);


	return (0);
}

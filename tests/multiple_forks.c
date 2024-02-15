/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_forks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 12:16:35 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/15 12:55:17 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int ac, char **ag, char **envp)
{
	int	id1 = fork();
	int	id2 = fork();
	int	status;

	if (id1 == 0)
	{
		if (id2 == 0)
		{
			printf("We are process Y id1=%d - id2=%d\n", id1, id2);
		}
		else
		{
			waitpid(id2, &status, 0);
			printf("We are process X id1=%d - id2=%d\n", id1, id2);
		}
	}
	else if (id2 == 0)
	{
		waitpid(id1, &status, 0);
		printf("We are process Z id1=%d - id2=%d\n", id1, id2);
	}
	else
	{
		waitpid(id1, &status, 0);
		waitpid(id2, &status, 0);
		printf("We are process parent id1=%d - id2=%d\n", id1, id2);
	}
	printf("END OF PROCESS id1=%d - id2=%d\n", id1, id2);
	return (0);
}

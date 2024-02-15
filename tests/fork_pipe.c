/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 12:16:35 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/15 14:49:06 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 *	We want to calculate the sum of the array elements using multiple processes
 */
int main(int ac, char **ag, char **envp)
{
	int	arr[] = { 1, 2, 3, 4, 1, 2, 8, 4, 9};
	int	array_size = sizeof(arr) / sizeof(int);
	int	start, end;
	int	fd[2];
	int	fd2[2];
	int	id;
	int	id2;

	if (pipe(fd) == -1)
		return (1);
	id = fork();
	if (id == -1)
		return (2);
	if (id != 0)
	{
		if (pipe(fd2) == -1)
		{
			close(fd[0]);
			close(fd[1]);
			wait(NULL);
			return (1);
		}
		id2 = fork();
		if (id2 == -1)
		{
			wait(NULL);
			return (2);
		}
	}
	if (id == 0)
	{
		start = 0;
		end = array_size / 3;
	}
	else if (id2 == 0)
	{
		start = array_size / 3;
		end = (array_size / 3) * 2;
	}
	else
	{
		wait(NULL);
		start = (array_size / 3) * 2;
		end = array_size;
	}
	int	sum = 0;
	int	i;
	for (i = start; i < end; i++)
		sum += arr[i];
	printf("SUM: %d\n", sum);
	if (id == 0)
	{
		close(fd[0]);
		if (write(fd[1], &sum, sizeof(sum)) == -1)
			return (3);
		close(fd[1]);
	}
	else if (id2 == 0)
	{
		close(fd2[0]);
		if (write(fd2[1], &sum, sizeof(sum)) == -1)
			return (3);
		close(fd2[1]);
	}
	else
	{
		int	tempSum;
		close(fd[1]);
		close(fd2[1]);
		if (read(fd[0], &tempSum, sizeof(sum)) == -1)
			return (4);
		close(fd[0]);
		sum = sum + tempSum;
		if (read(fd2[0], &tempSum, sizeof(sum)) == -1)
			return (4);
		close(fd2[0]);
		sum = sum + tempSum;

		printf("FINAL SUM: %d\n", sum);
		wait(NULL);
	}
	return (0);
}

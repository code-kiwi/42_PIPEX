/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 12:16:35 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/15 11:39:55 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int main(int ac, char **ag, char **envp)
{
	int	id1 = fork();
	int	id2 = fork();
	int	status;

	if (id1 == 0)
	{
		if (id2 == 0)
		{
			ft_printf("We are process Y id1=%d - id2=%d\n", id1, id2);
		}
		else
		{
			waitpid(id2, &status, 0);
			ft_printf("We are process X id1=%d - id2=%d\n", id1, id2);
		}
	}
	else if (id2 == 0)
	{
		waitpid(id1, &status, 0);
		ft_printf("We are process Z id1=%d - id2=%d\n", id1, id2);
	}
	else
	{
		waitpid(id1, &status, 0);
		waitpid(id2, &status, 0);
		ft_printf("We are process parent id1=%d - id2=%d\n", id1, id2);
	}
	ft_printf("END OF PROCESS id1=%d - id2=%d\n", id1, id2);
	return (0);
}

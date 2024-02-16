/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 12:17:27 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/16 14:22:20 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

# define ERROR_MESSAGE_ARGS "Usage: pipex infile cmd1 [...] cmdn outfile\n"

typedef struct s_pipex_data
{
	t_list	*commands;
	t_list	*pids;
	char	**paths;
	int		fd_infile;
	int		fd_outfile;
}	t_pipex_data;

void	init_pipex_data(t_pipex_data *data);
void	get_env_paths(t_pipex_data *data, char **envp);
void	get_commands(t_pipex_data *data, int argc, char **argv);
void	make_pipes(t_pipex_data *data);
void	exec_commands(t_pipex_data *data);
void	wait_pids(t_pipex_data *data);

#endif

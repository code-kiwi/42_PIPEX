/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 15:55:03 by mhotting          #+#    #+#             */
/*   Updated: 2024/02/27 19:33:37 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 *	Checks if the first argument given to the program is HERE_DOC_STR
 *	if not, just returns
 *	else, here_doc is required:
 *		- sets data->here_doc_active to true
 *		- opens a pipe with the write end used to read from STDIN_FILENO
 *		and the read end given as data->fd_infile in order to give it to
 *		the first command
 *		- when the here_doc_read function ends, the write end of the pipe
 *		is closed
 *	On error, the program leaves properly
 */
static void	handle_here_doc(t_pipex_data *data, int argc, char **argv)
{
	int	fds[2];

	if (data == NULL || argv == NULL || argc < 5)
		handle_error(data, false, ERROR_MESSAGE_NULL_PTR, NULL);
	if (ft_strcmp(argv[1], HERE_DOC_STR) != 0)
		return ;
	if (argc < 6)
		handle_error(data, false, ERROR_MESSAGE_ARGS, NULL);
	data->here_doc_active = true;
	if (pipe(fds) == -1)
		handle_error(data, true, NULL, NULL);
	data->fd_infile = fds[0];
	if (!here_doc_read(argv[2], fds[1]))
	{
		close_file(data, fds[1]);
		handle_error(data, true, NULL, NULL);
	}
	close_file(data, fds[1]);
}

/*
 *	Parses envp in order to get the PATH content into a NULL terminated array
 *	(saved into data->paths)
 *	Parses the argv commands in order to create a linked list with all the
 *	commands (saved into data->commands)
 *	On error, the program is exited properly, an appropriate message is prompted
 *	on STDERR
 */
static void	parse_args(t_pipex_data *data, int argc, char **argv, char **envp)
{
	if (data == NULL || argv == NULL || envp == NULL)
		handle_error(data, false, ERROR_MESSAGE_NULL_PTR, NULL);
	data->paths = get_env_paths(envp);
	if (data->paths == NULL)
		handle_error(data, true, NULL, NULL);
	data->commands = get_commands(argc, argv, data->here_doc_active);
	if (data->commands == NULL)
		handle_error(data, true, NULL, NULL);
}

/*
 *	Handles the given infile and the given outfile
 *	Opens them with the appropriate flags and saved their fds into
 *	data->fd_infile and fd_outfile
 *	If here_doc_active is true, the data->fd_infile is already set
 *	On error, the execution continues, -1 is the value saved and error messages
 *	are prompted
 */
static void	handle_files(t_pipex_data *data, char *infile, char *outfile)
{
	if (data == NULL || infile == NULL || outfile == NULL)
		handle_error(data, false, ERROR_MESSAGE_NULL_PTR, NULL);
	if (!data->here_doc_active)
	{
		data->fd_infile = handle_infile(infile);
		if (data->fd_infile == -1)
		{
			if (errno != 0)
				ft_dprintf(STDERR_FILENO, "%s: %s: %s\n", \
						data->program_name, strerror(errno), infile);
			else
				ft_dprintf(STDERR_FILENO, "%s: %s: %s\n", \
						data->program_name, "Unknown error", outfile);
		}
	}
	data->fd_outfile = handle_outfile(outfile, data->here_doc_active);
	if (data->fd_outfile == -1)
	{
		if (errno != 0)
			ft_dprintf(STDERR_FILENO, "%s: %s: %s\n", \
					data->program_name, strerror(errno), outfile);
		else
			ft_dprintf(STDERR_FILENO, "%s: %s: %s\n", \
					data->program_name, "Unknown error", outfile);
	}
}

/*
 *	Goes through each command of data->commands, sets its fds and executes it
 *	On error, the program exits properly, displaying an error message
 */
static void	handle_commands(t_pipex_data *data)
{
	t_list		*curr;
	t_command	*cmd;
	bool		ok;

	curr = data->commands;
	while (curr != NULL)
	{
		cmd = (t_command *)(curr->content);
		ok = set_cmd_fds(data, cmd, curr == data->commands, curr->next == NULL);
		if (!ok)
			handle_error(data, true, NULL, NULL);
		if (
			cmd->fd_in != -1 && cmd->fd_out != -1
			&& cmd->fd_in != FD_UNSET && cmd->fd_out != FD_UNSET
		)
			handle_command(data, cmd);
		close_cmd_fds(cmd);
		curr = curr->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex_data	data;
	bool			all_process_ok;

	if (argc < 5)
		handle_error(NULL, false, ERROR_MESSAGE_ARGS, NULL);
	init_pipex_data(&data, argv[0], envp);
	handle_here_doc(&data, argc, argv);
	parse_args(&data, argc, argv, envp);
	handle_files(&data, argv[1], argv[argc - 1]);
	handle_commands(&data);
	all_process_ok = wait_pids(data.commands);
	clean_pipex_data(&data);
	if (!all_process_ok)
		exit(EXIT_FAILURE);
	return (0);
}

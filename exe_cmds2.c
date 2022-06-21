/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmds2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocapers <ocapers@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:22:51 by ocapers           #+#    #+#             */
/*   Updated: 2022/06/21 18:18:37 by ocapers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_right_redir(t_redirs *elem, t_shell **shell)
{
	int	fd;

	fd = open(elem->word, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (fd == -1)
	{
		(*shell)->exit_status = 2;
		error("open\n");
	}
	dup2(fd, STDOUT_FILENO);
}

void	open_files(t_redirs *elem, t_shell **shell, int in)
{	
	int	fd;

	if (elem->type == 0)
		handle_right_redir(elem, shell);
	if (elem->type == 1)
		handle_left_redir(elem, shell);
	if (elem->type == 2)
	{
		fd = open(elem->word, O_CREAT | O_WRONLY | O_APPEND, 0777);
		if (fd == -1)
			error("open\n");
		dup2(fd, STDOUT_FILENO);
	}
	if (elem->type == 3)
	{
		signal(SIGINT, (void *)sig_heredoc);
		here_doc(elem->word, shell, in);
	}
}

pid_t	watch_child_process(t_shell **shell, t_cmds **cmds, int in, char **envp)
{
	int		counter;
	pid_t	pid;

	counter = 0;
	set_signals_two(cmds);
	while (cmds[counter])
	{
		pid = fork();
		if (pid == 0)
		{
			if ((*shell)->cmd_index > 1)
			{
				get_command_position(cmds[counter], shell, counter, in);
				close_all_pipes((*shell)->pipes);
			}
			else
				handle_redirects(cmds[counter], shell, in);
			launch_command(cmds[counter], envp, shell);
		}
		else if (pid == -1)
			fork_error(shell);
		counter++;
	}
	return_signals_parent_process();
	return (pid);
}

void	get_child_exit_status(int *status)
{
	if (WIFEXITED(*status))
		*status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
		*status = WTERMSIG(*status);
	else if (WIFSTOPPED(*status))
		*status = WSTOPSIG(*status);
}

void	wait_child_processes(t_shell **shell, pid_t id)
{
	pid_t	process;
	int		status;
	int		i;

	status = 0;
	i = 0;
	while (i < (*shell)->cmd_index)
	{
		process = waitpid(-1, &status, 0);
		if (id == process)
		{
			get_child_exit_status(&status);
			(*shell)->exit_status = status;
		}
		i++;
	}
}

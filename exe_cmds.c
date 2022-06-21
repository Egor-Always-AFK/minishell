/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocapers <ocapers@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:20:19 by ocapers           #+#    #+#             */
/*   Updated: 2022/06/21 16:23:20 by ocapers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_num_of_commands(t_cmds **cmds)
{
	int	i;

	i = 0;
	while (cmds && cmds[i])
		i++;
	return (i);
}

int	is_built_in(char *cmd)
{
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	return (0);
}

void	built_ins(t_envlist **list, t_cmds *cmds, t_shell **shell, char **envp)
{
	(void)envp;
	if (!cmds->args)
		return ;
	if (ft_strncmp(cmds->args->content, "pwd", 4) == 0)
		pwd_ms(shell, cmds->args, *list);
	else if (ft_strncmp(cmds->args->content, "echo", 5) == 0)
		echo_ms(cmds->args, shell);
	else if (ft_strncmp(cmds->args->content, "cd", 3) == 0)
		cd_ms(list, cmds->args, shell);
	else if (ft_strncmp(cmds->args->content, "env", 4) == 0)
		env_ms(*list, shell);
	else if (ft_strncmp(cmds->args->content, "exit", 5) == 0)
		exit_ms(shell, cmds->args);
	else if (ft_strncmp(cmds->args->content, "unset", 6) == 0)
		unset_ms(list, cmds->args, shell);
	else if (ft_strncmp(cmds->args->content, "export", 7) == 0)
		export_ms(list, cmds->args, shell);
}

void	here_doc(char *del, t_shell **shell, int in)
{
	char	*line;
	int		fd;

	(*shell)->exit_status = 0;
	fd = open("/tmp/file", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd == -1)
		error("open\n");
	while (true)
	{
		line = get_next_line(in);
		if (!line)
			break ;
		if (ft_strncmp(del, line, ft_strlen(del)) == 0
			&& ft_strlen(del) == ft_strlen(line) - 1)
			break ;
		write(fd, line, ft_strlen(line));
		free(line);
	}
	close(fd);
	fd = open("/tmp/file", O_RDONLY, 0777);
	if (fd == -1)
		error("open\n");
	dup2(fd, STDIN_FILENO);
}

void	handle_left_redir(t_redirs *elem, t_shell **shell)
{
	int	fd;

	fd = open(elem->word, O_RDONLY, 0777);
	if (fd == -1)
	{
		(*shell)->exit_status = 1;
		write(2, "minishell: ", 11);
		write(2, elem->word, ft_strlen(elem->word) + 1);
		write(2, ": No such file or directory\n", 28);
		exit(1);
	}
	dup2(fd, STDIN_FILENO);
}

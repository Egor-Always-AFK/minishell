/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmds4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocapers <ocapers@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 17:24:22 by ocapers           #+#    #+#             */
/*   Updated: 2022/06/21 18:18:45 by ocapers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_system_bin(t_cmds *cmd, char **path, char ***cmd_args)
{
	char	**paths;
	int		i;

	i = 0;
	paths = parse_paths(cmd->envs);
	*cmd_args = get_command_arguments(cmd->args);
	if (!(*cmd_args))
		error("Error\n");
	while (paths && paths[i])
	{
		*path = ft_strjoin(paths[i], (*cmd_args)[0]);
		if (!*path)
			error("Error\n");
		if (access(*path, F_OK) == 0)
			break ;
		free(paths[i]);
		free(*path);
		*path = NULL;
		i++;
	}
	if (!*path)
		handle_no_path(path, cmd_args);
	free(paths);
}

void	handle_empty_path(char *path, t_cmds *cmd, 
            char **cmd_args, t_shell **shell)
{
	if (path == NULL && !find_env(cmd->envs, "PATH"))
	{
		(*shell)->exit_status = 127;
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, cmd_args[0], ft_strlen(cmd_args[0]));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
		exit((*shell)->exit_status);
	}
	else if (path == NULL)
	{
		(*shell)->exit_status = 127;
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, cmd_args[0], ft_strlen(cmd_args[0]));
		write(STDERR_FILENO, ": cmd not found\n", 20);
		exit((*shell)->exit_status);
	}
}

void	my_exec(t_shell **shell, char *path, char **cmd_args, char **envp)
{
	int	exec_res;

	(*shell)->exit_status = 0;
	exec_res = execve(path, cmd_args, envp);
	if (exec_res == -1)
	{
		(*shell)->exit_status = 127;
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, cmd_args[0], ft_strlen(cmd_args[0]));
		write(STDERR_FILENO, ": cmd not found\n", 20);
		exit((*shell)->exit_status);
	}
	if (exec_res == -1 && cmd_args && *cmd_args)
	{
		(*shell)->exit_status = 127;
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, cmd_args[0], ft_strlen(cmd_args[0] + 1));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
		exit((*shell)->exit_status);
	}
}

void	launch_command(t_cmds *cmd, char **envp, t_shell **shell)
{
	char	*path;
	char	**cmd_args;

	path = NULL;
	cmd_args = get_command_arguments(cmd->args);
	if (!cmd_args)
		error("error");
	cmd->args->content = check_brackets(cmd);
	if (is_built_in(cmd->args->content))
	{
		built_ins(&(cmd->envs), cmd, shell, envp);
		exit(0);
	}
	else if (ft_strchr(cmd->args->content, '/') != NULL)
	{
		cmd_args = get_command_arguments(cmd->args);
		if (!*cmd_args)
			error("Error");
		path = &(*cmd_args)[0];
	}
	else
		exec_system_bin(cmd, &path, &cmd_args);
	handle_empty_path(path, cmd, cmd_args, shell);
	my_exec(shell, path, cmd_args, envp);
	(*shell)->exit_status = 1;
	exit((*shell)->exit_status);
}

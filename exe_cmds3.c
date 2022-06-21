#include "minishell.h"

char	*check_brackets(t_cmds *cmd)
{
	char	*str;
	char	*tmp;
	char	*ret;
	int		i;
	int		j;

	i = 0;
	j = 0;
	str = (char *)cmd->args->content;
	tmp = malloc(sizeof(char *) * 10);
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			i++;
		else
		{
			tmp[j] = str[i];
			i++;
			j++;
		}
	}
	return (tmp);
}

// int	handle_redirects(t_cmds *cmd, t_shell **shell, int in)
// {
// 	int	i;

// 	i = 0;
// 	while (cmd->redirs && cmd->redirs[i])
// 	{
// 		open_files(cmd->redirs[i], shell, in);
// 		i++;
// 	}
// 	return (0);
// }

// void	execute_cmds(t_cmds **cmds, t_shell **shell, char **envp, char *line)
// {
// 	int	in;
// 	int	out;

// 	envp = NULL;
// 	(*shell)->cmd_index = get_num_of_commands(cmds);
// 	if ((*shell)->cmd_index > 1)
// 		(*shell)->pipes = pipes_loop((*shell)->cmd_index);
// 	if (*line != '\0')
// 	{
// 		in = dup(STDIN_FILENO);
// 		out = dup(STDOUT_FILENO);
// 		if (is_built_in(cmds[0]->args->content) && !cmds[1])
// 		{
// 			handle_redirects(cmds[0], shell, in);
// 			built_ins(&(cmds[0]->envs), cmds[0], shell, envp);
// 		}
// 		else
// 		{
// 			execute_bin(cmds, shell, envp, in);
// 		}
// 		dup2(in, STDIN_FILENO);
// 		dup2(out, STDOUT_FILENO);
// 	}
// }

void	execute_bin(t_cmds **cmds, t_shell **shell, char **envp, int in)
{
	pid_t	pid;

	(*shell)->exit_status = 0;
	pid = watch_child_process(shell, cmds, in, envp);
	close_all_pipes(((*shell)->pipes));
	wait_child_processes(shell, pid);
}

void	fork_error(t_shell **shell)
{
	(*shell)->exit_status = 128;
	write(STDERR_FILENO, "minishell: fork: Error", 24);
	exit((*shell)->exit_status);
}

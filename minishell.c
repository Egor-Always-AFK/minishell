/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocapers <ocapers@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:33:05 by ocapers           #+#    #+#             */
/*   Updated: 2022/06/21 16:37:53 by ocapers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error(char *error)
{
	ft_putstr_fd(error, 2);
	exit(1);
}

void	init_shell(t_shell *shell, t_envlist *envlist)
{
	shell->level = set_level(envlist);
	shell->exit_status = 0;
	shell->pipes = NULL;
	shell->cmd_index = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_envlist	*envlist;
	t_shell		*shell;
	t_cmds		**cmds;
	char		*str;

	(void)argv;
	(void)argc;
	cmds = NULL;
	shell = malloc(sizeof(t_shell));
	envlist = envp_to_envlist(envp);
	init_shell(shell, envlist);
	rl_outstream = stderr;
	while (true)
	{
		tty_input();
		set_signals();
		str = readline("minishell> ");
		if (!str)
			error("exit\n");
		if (str && *str && *str != ' ')
			add_history(str);
		cmds = parser(str, envlist, shell);
		shell->cmd_index = get_num_of_commands(cmds);
		execute_cmds(cmds, &shell, envp, str);
		// commands_clear(&cmds);
		free(str);
	}
	return (0);
}

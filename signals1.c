#include "minishell.h"

void	*sig_fork(int num)
{
	(void)num;
	exit(2);
}

void	set_signals_two(t_cmds **commands)
{
	if (ft_strncmp(commands[0]->args->content, "cat", ft_strlen("cat")) == 0)
		signal(SIGQUIT, (void *)sigquit_handler);
	signal(SIGINT, (void *)sig_fork);
	signal(SIGQUIT, sigquit_handler);
}

void	return_signals_parent_process(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

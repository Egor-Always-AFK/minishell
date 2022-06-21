#include "minishell.h"

// char	*del_quotes(char *str, t_cmds *cmd)
// {
// 	int	i;

// 	i = -1;
// 	while (str && str[++i])
// 	{
// 		if (str[i] == '\'')
// 			str = strip_quotes(str, &i, '\'', cmd);
// 		else if (str[i] == '\"')
// 			str = strip_quotes(str, &i, '\"', cmd);
// 		else if (str[i] == '$')
// 			str = replace_path(str, &i, cmd);
// 	}
// 	return (str);
// }

// t_cmds	*get_cmd_two(t_cmds **cmd)
// {
// 	int		i;

// 	i = -1;
// 	while ((*cmd)->redirs && (*cmd)->redirs[++i])
// 		(*cmd)->redirs[i]->word = del_quotes((*cmd)->redirs[i]->word, *cmd);
// 	return (*cmd);
// }

// void	get_cmd_arguments(char *str, t_cmds *cmd)
// {
// 	t_list	*arg;
// 	int		i[5];
// 	char	*sub;

// 	ft_memset(i, 0, sizeof(int) * 5);
// 	while (str[i[0]])
// 	{
// 		quotes_check(&i[3], &i[4], str[i[0]]);
// 		if (!i[0] && str[i[0]] == ' ' && !i[1]++)
// 			;
// 		if (i[0] && !i[3] && !i[4] && str[i[0]] == ' ')
// 		{
// 			arg = ft_lstnew(ft_substr(str, i[1], i[0] - i[1]));
// 			if (!arg)
// 				error("Error (list cmd)\n");
// 			ft_lstadd_back(&cmd->args, arg);
// 			i[1] = i[0] + 1;
// 		}
// 		i[0]++;
// 	}
// 	sub = ft_substr(str, i[1], i[0] - i[1]);
// 	arg = ft_lstnew(ft_substr(str, i[1], i[0] - i[1]));
// 	if (!arg)
// 		error("Error (list cmd)\n");
// 	ft_lstadd_back(&cmd->args, arg);
// }

#include "minishell.h"

int	get_delta_of_indexes(char *str, int i, char **value, t_cmds *cmd)
{
	int	j;

	j = 1;
	if (str[i + 1] && ft_isdigit(str[i + 1]))
		j = 2;
	else if (str[i + 1] && str[i + 1] == '?')
	{
		if (cmd && cmd->shell)
				*value = ft_itoa(cmd->shell->exit_status);
		j = 2;
	}
	else
		while (str[++i] && !ft_strchr("\'\"$ =", str[i]))
			j++;
	return (j);
}

char	*replace_path(char *str, int *index, t_cmds *cmd)
{
	char		*value;
	t_envlist	*env;
	int			i;

	i = *index;
	value = NULL;
	env = cmd->envs;
	i += get_delta_of_indexes(str, i, &value, cmd);
	while (!value && env)
	{
		if (!ft_strncmp(&str[*index + 1], env->key, i - *index - 1)
			&& (i - *index - 1) == (int)ft_strlen(env->key))
			value = ft_strdup(env->value);
		env = env->next;
	}
	if (!value && (i - *index - 1))
		value = ft_strdup("");
	else if (!value && !(i - *index - 1))
		value = ft_strdup("$");
	value = ft_strjoin(ft_substr(str, 0, *index), value);
	*index = ft_strlen(value) - 1;
	value = ft_strjoin(value, ft_strdup(&str[i]));
	free(str);
	return (value);
}

char	*strip_quotes(char *str, int *index, char quote, t_cmds *cmd)
{
	int		i;
	char	*tmp[3];

	i = *index + 1;
	while (str[i] && str[i] != quote)
	{
		if (str[i] == '$' && quote != '\'')
		{
			str = replace_path(str, &i, cmd);
		}
		i++;
	}
	if (i - *index > 1)
	{
		tmp[0] = ft_substr(str, 0, *index);
		tmp[1] = ft_substr(str, *index + 1, i - *index - 1);
		tmp[2] = ft_strdup(&str[i + 1]);
		*index = ft_strlen(tmp[0]) + ft_strlen(tmp[1]) - 1;
	}
	else
	{
		tmp[0] = ft_substr(str, 0, *index);
		*index = ft_strlen(tmp[0]) - 1;
		tmp[1] = ft_strdup(&str[i + 1]);
		tmp[2] = NULL;
	}
	tmp[0] = ft_strjoin(tmp[0], tmp[1]);
	tmp[0] = ft_strjoin(tmp[0], tmp[2]);
	free(str);
	return (tmp[0]);
}

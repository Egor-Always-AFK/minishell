/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocapers <ocapers@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 18:13:01 by ocapers           #+#    #+#             */
/*   Updated: 2022/06/21 19:03:51 by ocapers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_ms(t_envlist *list, t_shell **shell)
{
	(*shell)->exit_status = 0;
	while (list)
	{
		write(STDOUT_FILENO, list->key, ft_strlen(list->key));
		write(STDOUT_FILENO, "=", 1);
		write(STDOUT_FILENO, list->value, ft_strlen(list->value));
		write(STDOUT_FILENO, "\n", 1);
		list = list->next;
	}
}

char	*find_env(t_envlist *list, const char *key)
{
	t_envlist	*tmp;
	char		*value;

	tmp = list;
	value = NULL;
	if (tmp == NULL)
		return (NULL);
	while (tmp)
	{
		if (ft_strncmp(tmp->key, key, ft_strlen((char *)key)) == 0
			&& ft_strlen(tmp->key) == ft_strlen((char *)key))
			value = tmp->value;
		tmp = tmp->next;
	}
	return (value);
}

void	pwd_ms(t_shell **shell, t_list *args, t_envlist *list)
{
	char	dir[4096];
	char	*tmp_dir;

	if (args->next)
	{
		(*shell)->exit_status = 1;
		write(STDERR_FILENO, "pwd: too many arguments\n", 25);
	}
	else
	{
		(*shell)->exit_status = 0;
		if (getcwd(dir, 4096) == NULL)
		{
			tmp_dir = find_env(list, "OLDPWD");
			write(1, tmp_dir, ft_strlen(tmp_dir) + 1);
		}
		write(STDOUT_FILENO, dir, ft_strlen(dir));
		write(STDOUT_FILENO, "\n", 1);
	}
}

int	echo_help(t_list *curr)
{
	int		i;
	int		flag;
	char	*str;
	int		tmp;
	int count;

	flag = 0;
	count = counter(curr);
	tmp = 0;
	while (curr && ft_strncmp(curr->content, "-n", ft_strlen("-n")) == 0)
	{
		flag = 1;
		curr = curr->next;
	}
	while (curr != NULL)
	{
		str = curr->content;
		i = 0;
		// printf("\n\n%s\n\n", curr->content);
		if ( tmp != 0 && curr && curr->content && (((char *)curr->content)[0]))
			write(STDOUT_FILENO, " ", 1);
		if (str[0] == '\'' || str[0] == '\"')
		{
			i = 1;
			while (i < (ft_strlen(str) - 1))
			{
				if (str[i] == '\'' || str[i] == '\"')
					i++;
				else
				{
					write(STDOUT_FILENO, &str[i], 1);
					i++;
				}
			}
		}
		else
			write(STDOUT_FILENO, &str[i], (ft_strlen(str)));
		tmp += 1;
		curr = curr->next;
	}
	return (flag);
}

void	echo_ms(t_list *args, t_shell **shell)
{
	int		flag;
	t_list	*curr;

	curr = args->next;
	if (!curr && (*shell)->exit_status != 1)
	{
		write(1, "\n", 1);
		return ;
	}
	else if (!curr)
		return ;
	flag = echo_help(curr);
	if (!flag)
		write(STDOUT_FILENO, "\n", 1);
	(*shell)->exit_status = 0;
}

int counter(t_list *str)
{
	int i;
	t_list *tmp;
	
	tmp = str;
	i = 0;
	while (tmp != NULL)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}
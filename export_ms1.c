#include "minishell.h"

char	**handle_export_without_args(t_envlist *list)
{
	int		size;
	char	**tmp;
	int		i;

	size = get_list_size(list);
	tmp = (char **)malloc(sizeof(char *) * (size + 1));
	if (!tmp)
		error("Error malloc\n");
	i = 0;
	while (list)
	{
		tmp[i] = ft_strjoin(list->key, "=");
		tmp[i] = ft_strjoin(tmp[i], list->value);
		if (tmp[i] == NULL)
			error("Error malloc\n");
		list = list->next;
		i++;
	}
	tmp[i] = NULL;
	tmp = get_sorted_keys(tmp, size);
	return (tmp);
}

void	display_sorted_list(char **sorted_keys)
{
	int	i;

	i = 0;
	while (sorted_keys[i] != NULL)
	{
		write(STDOUT_FILENO, "declare -x ", 11);
		write(STDOUT_FILENO, sorted_keys[i], ft_strlen(sorted_keys[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}

void	add_env_key_value_pair(t_envlist **list, char *arg)
{
	char		**key_value;
	t_envlist	*new_node;

	key_value = ft_split(arg, '=');
	if (!key_value)
		error("Error malloc\n");
	new_node = ft_envar_new(key_value[0], key_value[1]);
	if (!new_node)
		error("Error malloc\n");
	ft_envar_add_back(list, new_node);
	free(key_value);
}

void	handle_export_with_arguments(t_list *curr,
		t_shell **shell, t_envlist **list)
{
	while (curr)
	{
		if (!is_valid_env_key(curr->content))
		{
			(*shell)->exit_status = 1;
			write(STDERR_FILENO, "minishell: export: `", 20);
			write(STDERR_FILENO, curr->content, ft_strlen(curr->content));
			write(STDERR_FILENO, "': not a valid identifier\n", 27);
		}
		else if (!is_equal_sign(curr->content))
			(*shell)->exit_status = 0;
		else
		{	
			(*shell)->exit_status = 0;
			check_if_key_exists(list, curr->content);
			add_env_key_value_pair(list, curr->content);
		}
		curr = curr->next;
	}
}

void	export_ms(t_envlist **list, t_list *args, t_shell **shell)
{
	char	**sorted_keys;
	t_list	*curr;

	sorted_keys = NULL;
	curr = args->next;
	if (!curr)
	{
		(*shell)->exit_status = 0;
		sorted_keys = handle_export_without_args(*list);
		display_sorted_list(sorted_keys);
	}
	else
		handle_export_with_arguments(curr, shell, list);
}

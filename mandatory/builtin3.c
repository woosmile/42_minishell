/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:10:43 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/02 20:48:16 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export(t_cmd *cmd, t_env *env_head)
{
	int		i;
	char	*name;
	char	*value;
	int		check;

	if (cmd->argv[1] == NULL)
		export_no_arg(env_head);
	i = 0;
	check = 0;
	while (cmd->argv[++i])
	{
		name = env_find_name(cmd->argv[i]);
		if (is_valid_name(name) == 0)
		{
			check = export_syntax_error(name);
			continue ;
		}
		value = env_find_value(cmd->argv[i]);
		env_set_value(env_head, name, value);
		free(name);
		free(value);
	}
	return (check);
}

int	export_no_arg(t_env *env_head)
{
	t_env	*env_iter;

	env_iter = env_head;
	while (env_iter)
	{
		printf("declare -x %s", env_iter->name);
		if (env_iter->value != NULL)
			printf("=\"%s\"\n", env_iter->value);
		else
			printf("\n");
		env_iter = env_iter->next;
	}
	return (0);
}

int	export_syntax_error(char *name)
{
	ft_putstr_fd("minishell: export: ", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putstr_fd(" : not a valid identifier\n", STDERR_FILENO);
	free(name);
	return (1);
}

int	ft_unset(t_cmd *cmd, t_env **env_head)
{
	int	i;
	int	check;

	i = 1;
	check = 0;
	while (cmd->argv[i])
	{
		if (is_valid_name(cmd->argv[i]) == 0)
		{
			ft_putstr_fd("minishell: unset: '", STDERR_FILENO);
			ft_putstr_fd(cmd->argv[i], STDERR_FILENO);
			ft_putstr_fd(" : not a valid identifier\n", STDERR_FILENO);
			check = 1;
		}
		else
			env_remove(env_head, cmd->argv[i]);
		i++;
	}
	return (check);
}

int	ft_env(t_env *env_head)
{
	t_env	*env_iter;

	env_iter = env_head;
	while (env_iter)
	{
		if (env_iter->value != NULL)
		{
			ft_putstr_fd(env_iter->name, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putendl_fd(env_iter->value, STDOUT_FILENO);
		}
		env_iter = env_iter->next;
	}
	return (0);
}

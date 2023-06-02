/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:10:43 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/02 18:25:59 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_set_value(t_env *env_head, char *name, char *value)
{
	t_env	*env_iter;
	t_env	*new;
	char	*new_value;

	new_value = env_set_new_value(value);
	env_iter = env_head;
	while (env_iter)
	{
		if (strcmp(env_iter->name, name) == 0)
		{
			free(env_iter->value);
			env_iter->value = new_value;
			return (0);
		}
		if (env_iter->next == NULL)
			break ;
		env_iter = env_iter->next;
	}
	name = ft_strdup(name);
	if (name == NULL)
		exit (EXIT_FAILURE);
	new = new_env_node(name, new_value, env_iter);
	env_iter->next = new;
	return (0);
}

char	*env_set_new_value(char *value)
{
	char	*new_value;

	new_value = NULL;
	if (value != NULL)
	{
		new_value = ft_strdup(value);
		if (new_value == NULL)
			exit(EXIT_FAILURE);
	}
	return (new_value);
}

char	*env_get_value(t_env *env_head, char *name)
{
	t_env	*env_iter;
	char	*result;

	env_iter = env_head;
	while (env_iter)
	{
		if (strcmp(env_iter->name, name) == 0)
		{
			if (env_iter->value == NULL)
				return (NULL);
			result = ft_strdup(env_iter->value);
			if (result == NULL)
				exit (EXIT_FAILURE);
			return (result);
		}
		env_iter = env_iter->next;
	}
	return (NULL);
}

int	env_remove(t_env **env_head, char *name)
{
	t_env	*env_to_clear;

	if (*env_head == NULL)
		return (0);
	env_to_clear = env_find_env_to_remove(env_head, name);
	if (env_to_clear != NULL)
	{
		free(env_to_clear->name);
		free(env_to_clear->value);
		free(env_to_clear);
	}
	return (0);
}

t_env	*env_find_env_to_remove(t_env **env_head, char *name)
{
	t_env	*env_iter;
	t_env	*env_to_clear;

	env_to_clear = NULL;
	if (strcmp((*env_head)->name, name) == 0)
	{
		env_to_clear = *env_head;
		*env_head = (*env_head)->next;
		(*env_head)->prev = NULL;
	}
	env_iter = *env_head;
	while (env_iter->next)
	{
		if (strcmp(env_iter->next->name, name) == 0)
		{
			env_to_clear = env_iter->next;
			env_iter->next = env_to_clear->next;
			if (env_to_clear->next != NULL)
				env_to_clear->next->prev = env_iter;
			break ;
		}
		env_iter = env_iter->next;
	}
	return (env_to_clear);
}

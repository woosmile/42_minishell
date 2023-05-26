/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:10:43 by joonhlee          #+#    #+#             */
/*   Updated: 2023/05/25 00:11:11 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*env_get_value(t_env *env_head, char *name)
{
	t_env	*env_iter;

	env_iter = env_head;
	while (env_iter)
	{
		if (strcmp(env_iter->name, name) == 0)
			return (env_iter->value);
		env_iter = env_iter->next;
	}
	return (NULL);
}

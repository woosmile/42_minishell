/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_conv_arr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 19:50:32 by woosekim          #+#    #+#             */
/*   Updated: 2023/06/02 17:51:12 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	input_name_content(t_env *env_head, char *env_arr, int size)
{
	int		i;
	char	*name_equal;
	char	*str_total;

	name_equal = ft_strjoin(env_head->name, "=");
	if (!name_equal)
		exit (EXIT_FAILURE);
	str_total = ft_strjoin(name_equal, env_head->value);
	if (!str_total)
		exit (EXIT_FAILURE);
	i = 0;
	while (i < size - 1)
	{
		env_arr[i] = str_total[i];
		i++;
	}
	env_arr[i] = 0;
	free(name_equal);
	free(str_total);
}

int	env_list_len(t_env *env_head)
{
	int	len;

	len = 0;
	while (env_head != NULL)
	{
		env_head = env_head->next;
		len++;
	}
	return (len);
}

char	**env_conv_arr(t_env *env_head)
{
	int		i;
	int		len;
	char	**env_arr;

	len = env_list_len(env_head);
	env_arr = (char **)malloc(sizeof(char *) * (len + 1));
	if (!env_arr)
		exit (EXIT_FAILURE);
	i = 0;
	while (env_head != NULL)
	{
		len = ft_strlen(env_head->name) + ft_strlen(env_head->value) + 2;
		env_arr[i] = (char *)malloc(sizeof(char) * len);
		if (!env_arr[i])
			split_null_guard(env_arr, i);
		input_name_content(env_head, env_arr[i], len);
		env_head = env_head->next;
		i++;
	}
	env_arr[i] = 0;
	return (env_arr);
}

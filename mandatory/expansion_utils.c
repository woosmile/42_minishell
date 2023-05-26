/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 14:27:56 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/25 15:31:57 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	envp_name_input(t_token *temp, char *envp_name, \
						int name_len, size_t *w_idx)
{
	int	n_idx;

	n_idx = 0;
	while (n_idx < name_len)
	{
		envp_name[n_idx] = temp->str[*w_idx];
		n_idx++;
		(*w_idx)++;
	}
	envp_name[name_len] = 0;
}

void	envp_name(t_token *temp, int *word_rec, \
				char **envp_bundle, size_t str_len)
{
	int		name_len;
	size_t	w_idx;
	int		b_idx;

	w_idx = 0;
	b_idx = 0;
	while (w_idx < str_len)
	{
		if (word_rec[w_idx] == '$')
		{
			w_idx++;
			name_len = 0;
			while (word_rec[w_idx] == 0 && w_idx < str_len)
			{
				name_len++;
				w_idx++;
			}
			envp_bundle[b_idx] = (char *)malloc(sizeof(char) * (name_len + 1));
			w_idx = w_idx - name_len;
			envp_name_input(temp, envp_bundle[b_idx], name_len, &w_idx);
			b_idx++;
		}
		else
			w_idx++;
	}
}

void	envp_value(t_env *env_head, char **envp_bundle, int envp_arr_cnt)
{
	char	*temp_value;
	int		b_idx;

	b_idx = 0;
	while (b_idx < envp_arr_cnt)
	{
		temp_value = env_get_value(env_head, envp_bundle[b_idx]);
		free(envp_bundle[b_idx]);
		if (temp_value)
			envp_bundle[b_idx] = ft_strdup(temp_value);
		else
			envp_bundle[b_idx] = temp_value;
		b_idx++;
	}
}

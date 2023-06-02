/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 14:27:56 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/29 15:18:37 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_name_input(char *str, char *env_name, int name_len, size_t *w_idx)
{
	int	n_idx;

	n_idx = 0;
	while (n_idx < name_len)
	{
		env_name[n_idx] = str[*w_idx];
		n_idx++;
		(*w_idx)++;
	}
	env_name[name_len] = 0;
}

int	env_name_len(char *word_rec, size_t *w_idx)
{
	int	name_len;

	name_len = 0;
	if (word_rec[*w_idx] == '?')
	{
		name_len = 1;
		(*w_idx)++;
	}
	else
	{
		while (word_rec[*w_idx] == 0 && word_rec[*w_idx] != -1)
		{
			name_len++;
			(*w_idx)++;
		}
	}
	return (name_len);
}

void	env_name(char *str, char *word_rec, char **exp_bundle)
{
	int		name_len;
	size_t	w_idx;
	int		b_idx;

	w_idx = 0;
	b_idx = 0;
	while (word_rec[w_idx] != -1)
	{
		if (word_rec[w_idx] == '$')
		{
			w_idx++;
			name_len = env_name_len(word_rec, &w_idx);
			exp_bundle[b_idx] = (char *)malloc(sizeof(char) * (name_len + 1));
			if (split_null_guard(exp_bundle, b_idx))
				exit(1);
			w_idx = w_idx - name_len;
			env_name_input(str, exp_bundle[b_idx], name_len, &w_idx);
			b_idx++;
		}
		else
			w_idx++;
	}
}

void	env_value(t_env *env_head, char **exp_bundle, \
					int exp_arr_cnt, int b_idx)
{
	char	*temp_value;

	while (b_idx < exp_arr_cnt)
	{
		if (ft_strncmp("?", exp_bundle[b_idx], 2) == 0)
		{
			free(exp_bundle[b_idx]);
			exp_bundle[b_idx] = ft_itoa(g_exit_status);
			if (split_null_guard(exp_bundle, b_idx))
				exit(1);
		}
		else
		{
			temp_value = env_get_value(env_head, exp_bundle[b_idx]);
			free(exp_bundle[b_idx]);
			exp_bundle[b_idx] = temp_value;
		}
		b_idx++;
	}
}

char	**env_arr(char *str, t_env *env_head, char *word_rec)
{
	char	**exp_bundle;
	int		exp_arr_cnt;
	size_t	i;

	i = 0;
	exp_arr_cnt = 0;
	while (word_rec[i] != -1)
	{
		if (word_rec[i] == '$')
			exp_arr_cnt++;
		i++;
	}
	if (exp_arr_cnt == 0)
		return (NULL);
	exp_bundle = (char **)malloc(sizeof(char *) * (exp_arr_cnt + 1));
	if (!exp_bundle)
		exit(1);
	exp_bundle[exp_arr_cnt] = NULL;
	env_name(str, word_rec, exp_bundle);
	env_value(env_head, exp_bundle, exp_arr_cnt, 0);
	return (exp_bundle);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_str_combine.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:37:37 by woosekim          #+#    #+#             */
/*   Updated: 2023/06/02 18:26:15 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	front_str_combine(char *str, int s_idx, char *exp_bundle, \
						char **str_comb)
{
	char	*str_new;
	char	*old_str;

	str_new = ft_substr(str, 0, s_idx);
	if (!str_new)
		exit(1);
	if (exp_bundle != NULL)
	{
		old_str = str_new;
		str_new = ft_strjoin(str_new, exp_bundle);
		if (!str_new)
			exit(1);
		free(old_str);
	}
	if (!*str_comb)
		*str_comb = str_new;
	else
	{
		old_str = *str_comb;
		*str_comb = ft_strjoin(*str_comb, str_new);
		if (!str_comb)
			exit(1);
		free(old_str);
		free(str_new);
	}
}

char	*rear_str_combine(char *str, int s_idx, char *str_comb)
{
	char	*str_new;
	char	*str_old;

	if (s_idx > 0)
	{
		str_new = ft_substr(str, 0, s_idx);
		if (!str_new)
			exit(1);
		if (!str_comb)
			str_comb = str_new;
		else
		{
			str_old = str_comb;
			str_comb = ft_strjoin(str_comb, str_new);
			if (!str_comb)
				exit(1);
			free(str_old);
			free(str_new);
		}
	}
	return (str_comb);
}

char	*idx_init(char *word_rec, int *w_idx, char *str, int *s_idx)
{
	(*w_idx)++;
	if (word_rec[*w_idx] == '?')
		*w_idx += 1;
	else
	{
		while (word_rec[*w_idx] == 0)
			(*w_idx)++;
	}
	str = str + *w_idx;
	*s_idx = 0;
	return (str);
}

char	*exp_str_combine(char *str, char **exp_bundle, \
						char *word_rec, char *str_comb)
{
	int		w_idx;
	int		s_idx;
	int		b_idx;
	char	*str_origin;

	w_idx = 0;
	s_idx = 0;
	b_idx = 0;
	str_origin = str;
	while (word_rec[w_idx] != -1)
	{
		if (word_rec[w_idx] == '$')
		{
			front_str_combine(str, s_idx, exp_bundle[b_idx], &str_comb);
			b_idx++;
			str = idx_init(word_rec, &w_idx, str_origin, &s_idx);
		}
		else
		{
			w_idx++;
			s_idx++;
		}
	}
	str_comb = rear_str_combine(str, s_idx, str_comb);
	return (str_comb);
}

char	*exp_str_to_str(char *str, t_env *env_head)
{
	char	*word_rec;
	char	**exp_bundle;
	char	*str_comb;

	word_rec = word_type_recorder(str, 0, 0);
	exp_bundle = env_arr(str, env_head, word_rec);
	str_comb = exp_str_combine(str, exp_bundle, word_rec, NULL);
	free(word_rec);
	free_double_ptr(exp_bundle);
	return (str_comb);
}

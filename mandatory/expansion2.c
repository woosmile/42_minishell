/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 16:12:27 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/01 19:43:30 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*front_str_combine(char *str, int s_idx, char *exp_bundle, char *str_comb)
{
	char	*str_new;
	char	*old_str;

	str_new = ft_substr(str, 0, s_idx);
	if (exp_bundle != NULL)
	{
		old_str = str_new;
		str_new = ft_strjoin(str_new, exp_bundle);
		free(old_str);
	}
	if (!str_comb)
		str_comb = str_new;
	else
	{
		old_str = str_comb;
		str_comb = ft_strjoin(str_comb, str_new);
		free(old_str);
		free(str_new);
	}
	return (str_comb);
}

char	*rear_str_combine(char *str, int s_idx, char *str_comb)
{
	char	*str_new;
	char	*str_old;

	if (s_idx > 0)
	{
		str_new = ft_substr(str, 0, s_idx);
		if (!str_comb)
			str_comb = str_new;
		else
		{
			str_old = str_comb;
			str_comb = ft_strjoin(str_comb, str_new);
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

char *str_combine(char *str, char **exp_bundle, char *word_rec, char *str_comb)
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
			str_comb = front_str_combine(str, s_idx, exp_bundle[b_idx], str_comb);
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

//iterate str and find env name in list, replace by env_value
char *exp_str_to_str(char *str, t_env *env_head)
{
	char	*word_rec;
	char	**exp_bundle;
	char	*str_comb;

	word_rec = word_type_recorder(str, 0, 0);
	exp_bundle = env_arr(str, env_head, word_rec);
	str_comb = str_combine(str, exp_bundle, word_rec, NULL);
	free(word_rec);
	free_double_ptr(exp_bundle);
	return (str_comb);
}
/*
//token->prev > sublst > token->next token value is replaced by sublst_head
t_token *exp_connect(t_token *token, t_token *sublst_head);
t_token_type *check_ingredient2(char **split, t_token_type type);
int		find_n_split(char **split);

void	expansion(t_token *token_head, t_env *env_head)
{
	t_token	*token_iter;
	char	*str_with_value;
	t_token	*sublst_head;

	token_iter = token_head;
	while (token_iter)
	{
		if (token_iter->type == HEREDOC | token_iter->type == PIPE)
			token_iter = token_iter->next;
		else
		{
			str_with_value = exp_str_to_str(token_iter->str, env_head);
			sublst_head = tokenize_str(str_with_value, token_iter);
			token_iter = exp_connect(token_iter, sublst_head);
		}
	}
}

t_token	*tokenize_str(char *str_w_value, t_token *token)
{
	char			**split;
	t_token			*token_new_head;
	t_token_type	*type_arr;
	int				n_split;

	split = split_str(str_w_value, 0, 0, NULL);
	if (!split)
		exit (1);
	n_split = find_n_split(split);
	if (n_split > 2 && (token->type == INFILE || token->type == OUTFILE || token->type == APPEND))
	{
		token_new_head = new_token_node(AERROR, token->str, NULL);
		return (token_new_head);
	}
	type_arr = check_ingredient2(split, token->type);
	if (!type_arr)
	{
		free_double_ptr(split);
		exit (1);
	}
	token_new_head = token_list_init(split, type_arr, NULL, NULL);
	free_double_ptr(split);
	free(type);
}
*/
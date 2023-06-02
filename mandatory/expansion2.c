/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 16:12:27 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/02 10:44:47 by joonhlee         ###   ########.fr       */
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

//token->prev > sublst > token->next token value is replaced by sublst_head


void	expansion(t_token *token_head, t_env *env_head)
{
	t_token	*token_iter;
	char	*str_with_value;
	t_token	*sublst_head;

	token_iter = token_head;
		// printf("expansion2:131 before\n");
		// test_print_tokens(token_iter);
	while (token_iter)
	{
		if (token_iter->type == HEREDOC | token_iter->type == PIPE)
			token_iter = token_iter->next;
		else
		{
			str_with_value = exp_str_to_str(token_iter->str, env_head);
			printf("expansion2:138:str_w_value:|%s|\n", str_with_value);
			sublst_head = tokenize_str(str_with_value, token_iter);
			token_iter = exp_connect(token_iter, sublst_head);
		}
		// printf("expansion2:141 after\n");
		// test_print_tokens(token_iter);
		// test_print_tokens(token_head);
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
	if (n_split != 1 && (token->type == INFILE || token->type == OUTFILE
			|| token->type == APPEND))
	{
		free_double_ptr(split);
		token_new_head = new_token_node(AERROR, token->str, NULL);
		return (token_new_head);
	}
	else if (n_split == 0)
		return (NULL);
	type_arr = check_ingredient2(split, token->type);
	token_new_head = token_list_init(split, type_arr, NULL, NULL);
	free_double_ptr(split);
	free(type_arr);
	return (token_new_head);
}

t_token_type	*check_ingredient2(char **split, t_token_type type)
{
	int				len;
	int				i;
	t_token_type	*result;

	len = find_n_split(split);
	result = (t_token_type *)malloc((len) * sizeof (t_token_type));
	if (result == NULL)
		exit (1);
	i = 0;
	while (i < len)
		result[i++] = type;
	return (result);
}

int	find_n_split(char **split)
{
	int	len;

	len = 0;
	while (split[len])
	{
		len++;
	}
	return (len);
}

t_token	*exp_connect(t_token *token, t_token *sublst_head)
{
	t_token	*sublst_tail;
	t_token	*next;

	if (sublst_head == NULL)
	{
		free(token->str);
		token->str = NULL;
		return (token->next);
	}
	if (sublst_head->next == NULL)
	{
		free(token->str);
		token->str = sublst_head->str;
		free(sublst_head);
		return (token->next);
	}
	sublst_tail = sublst_head;
	while (sublst_tail->next)
		sublst_tail = sublst_tail->next;
	token->type = sublst_head->type;
	free(token->str);
	token->str = sublst_head->str;
	next = token->next;
	token->next = sublst_head->next;
	sublst_head->next->prev = token;
	free(sublst_head->str);
	free(sublst_head);
	sublst_tail->next = next;
	if (next != NULL)
		next->prev = sublst_tail;
	return (next);
}

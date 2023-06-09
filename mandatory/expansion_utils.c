/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:41:28 by woosekim          #+#    #+#             */
/*   Updated: 2023/06/09 14:38:28 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	word_type_recording(char *str, char *word_rec, \
							size_t i, int exp_flag)
{
	static int		dollar_flag;

	if (str[i] == '$' && exp_flag == 1 && dollar_flag == 0)
	{
		word_rec[i] = str[i];
		dollar_flag = 1;
	}
	else if ((str[i] == '$' && exp_flag == 1) || \
			str[i] == '\"' || str[i] == '\'' || \
			str[i] == '>' || str[i] == '<' || \
			str[i] == '|' || str[i] == '?' || \
			str[i] == ' ' || str[i] == '\t')
	{
		word_rec[i] = str[i];
		dollar_flag = 0;
	}
	if (i == ft_strlen(str) - 1)
		dollar_flag = 0;
}

char	*word_type_recorder(char *str, size_t str_len, size_t i)
{
	char		*word_rec;
	int			exp_flag;
	t_quotes	q;

	str_len = ft_strlen(str);
	word_rec = (char *)ft_calloc(str_len + 1, sizeof(char));
	if (!word_rec)
		exit(1);
	q.detect = 0;
	while (i < str_len)
	{
		exp_flag = 1;
		check_quotes(str[i], &q);
		if (q.detect == 1 && q.type == '\'')
			exp_flag = 0;
		word_type_recording(str, word_rec, i, exp_flag);
		i++;
	}
	word_rec[i] = -1;
	return (word_rec);
}

t_token_type	*check_ingredient_exp_arr(char **split, t_token_type type)
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

t_token	*tokenize_str(char *str_w_value, t_token *token)
{
	char			**split;
	t_token			*token_new_head;
	t_token_type	*type_arr;
	int				n_split;

	split = split_str(str_w_value, 0, 0, 1);
	free(str_w_value);
	if (!split)
		exit (1);
	n_split = find_n_split(split);
	if (n_split != 1 && (token->type == INFILE || token->type == OUTFILE \
						|| token->type == APPEND))
	{
		free_double_ptr(split);
		token_new_head = new_token_node(AERROR, token->str, NULL);
		return (token_new_head);
	}
	else if (n_split == 0)
		return (free_double_ptr(split));
	type_arr = check_ingredient_exp_arr(split, token->type);
	token_new_head = token_list_init(split, type_arr, NULL, NULL);
	free_double_ptr(split);
	free(type_arr);
	return (token_new_head);
}

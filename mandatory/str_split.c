/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 16:59:14 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/19 20:07:46 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	string_count(char const *s, char *word_rec, int exp_flag)
{
	int			i;
	size_t		cnt;
	t_quotes	q;

	i = 0;
	cnt = 0;
	q.detect = 0;
	while (s[i] != 0)
	{
		check_quotes(s[i], &q);
		if (!q.detect)
		{
			if (exp_flag == 0)
				check_metacharacter(s + i, &i, &cnt, word_rec);
			else
				check_exp_metacharacter(s + i, &i, &cnt, word_rec);
		}
		i++;
	}
	return (cnt);
}

int	word_count(char **str_arr, size_t str_cnt, char *word_rec)
{
	size_t		arr_i;
	size_t		rec_i;
	size_t		len;

	arr_i = 0;
	rec_i = 0;
	len = 0;
	while (arr_i < str_cnt)
	{
		if (word_rec[rec_i] == 0)
			len++;
		if (word_rec[rec_i] == 1)
		{
			len++;
			str_arr[arr_i] = (char *)malloc((len + 1) * sizeof(char));
			if (split_null_guard(str_arr, arr_i))
				return (0);
			arr_i++;
			len = 0;
		}
		rec_i++;
	}
	return (1);
}

void	word_input(char const *s, char **str_arr, \
					size_t str_cnt, char *word_rec)
{
	size_t	arr_i;
	size_t	arr_j;
	size_t	rec_i;

	arr_i = 0;
	arr_j = 0;
	rec_i = 0;
	while (arr_i < str_cnt)
	{
		if (word_rec[rec_i] == 0)
		{
			str_arr[arr_i][arr_j] = *s;
			arr_j++;
		}
		if (word_rec[rec_i] == 1)
		{
			str_arr[arr_i][arr_j] = *s;
			str_arr[arr_i][++arr_j] = 0;
			arr_i++;
			arr_j = 0;
		}
		s++;
		rec_i++;
	}
}

char	**split_str(char *s, size_t str_cnt, size_t s_len, int exp_flag)
{
	char	*word_rec;
	char	**str_arr;

	s_len = ft_strlen(s);
	word_rec = (char *)ft_calloc(s_len, sizeof(char));
	if (!word_rec)
		return (NULL);
	if (blank_masking(s, s_len, word_rec) == 1)
	{
		free(word_rec);
		return (NULL);
	}
	str_cnt = string_count(s, word_rec, exp_flag);
	str_arr = (char **)malloc((str_cnt + 1) * sizeof(char *));
	if (!str_arr)
		return (NULL);
	if (!word_count(str_arr, str_cnt, word_rec))
	{
		free(str_arr);
		return (NULL);
	}
	word_input(s, str_arr, str_cnt, word_rec);
	str_arr[str_cnt] = 0;
	free(word_rec);
	return (str_arr);
}

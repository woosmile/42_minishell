/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_str_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 13:24:20 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/19 19:38:53 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_sep(char c, char *charset)
{
	if (c == '\0')
		return (1);
	while (*charset != '\0')
	{
		if (c == *charset)
			return (1);
		charset++;
	}
	return (0);
}

void	check_metacharacter(const char *s, int *i, size_t *cnt, int *word_rec)
{
	char	*charset;

	charset = "<>| ";
	if (!ft_strncmp(s, "<", 1))
	{
		if (!ft_strncmp(s + 1, "<", 1))
			(*i)++;
		(*cnt)++;
		word_rec[*i] = 1;
	}
	else if (!ft_strncmp(s, ">", 1))
	{
		if (!ft_strncmp(s + 1, ">", 1))
			(*i)++;
		(*cnt)++;
		word_rec[*i] = 1;
	}
	else if (!ft_strncmp(s, "|", 1) || \
			(!check_sep(*s, charset) && check_sep(*(s + 1), charset)))
	{
		(*cnt)++;
		word_rec[*i] = 1;
	}
}

void	check_quotes(char s, char *quotes_type, int *detect)
{
	if (*detect == 0 && (s == '\'' || s == '\"'))
	{
		*detect = 1;
		*quotes_type = s;
	}
	else if (*detect == 1 && (s == *quotes_type))
		*detect = 0;
}

int	split_null_guard(char **str_arr, int i)
{
	if (!str_arr[i])
	{
		while (i > 0)
			free(str_arr[--i]);
		return (1);
	}
	return (0);
}

int	blank_masking(char *s, size_t s_len, int *word_rec)
{
	size_t	i;
	char	quotes_type;
	int		detect;

	i = 0;
	detect = 0;
	while (i < s_len)
	{
		check_quotes(s[i], &quotes_type, &detect);
		if (detect == 0)
		{
			if (s[i] == ' ')
				word_rec[i] = ' ';
		}
		i++;
	}
	if (detect)
		return (error_handling(1));
	return (0);
}

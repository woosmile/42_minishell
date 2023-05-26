/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_renew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 14:33:24 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/26 01:31:59 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	idx_shift(int mode, int *word_rec, size_t *n_idx, size_t *idx)
{
	if (mode == 0)
	{
		if (word_rec[*idx] == '?')
		{
			*n_idx = *n_idx + 2;
			(*idx)++;
		}
		while (word_rec[*idx] == 0)
			(*idx)++;
	}
	else if (mode == 1)
	{
		while (word_rec[*idx] == 0)
			(*idx)++;
	}
}

void	exit_status_input(int *word_rec, char *new_str, \
						size_t *n_idx, size_t *idx)
{
	if (word_rec[*idx] == '?')
	{
		(*idx)++;
		new_str[*n_idx] = '$';
		(*n_idx)++;
		new_str[*n_idx] = '?';
		(*n_idx)++;
	}
}

/*
char	*envp_value_select(char **envp_bundle, int *envp_idx)
{
	char	*envp_value;

	envp_value = envp_bundle[*envp_idx];
	(*envp_idx)++;
	return (envp_value);
}
*/

void	envp_value_input(char **envp_bundle, int *envp_idx, \
						char *new_str, size_t *n_idx)
{
	int		value_idx;
	char	*envp_value;
	int		value_len;

	value_idx = 0;
	if (envp_bundle[*envp_idx] == NULL)
	{
		(*envp_idx)++;
		return ;
	}
	envp_value = envp_bundle[*envp_idx];
	value_len = ft_strlen(envp_value);
	while (value_idx < value_len)
	{
		new_str[*n_idx] = envp_value[value_idx];
		(*n_idx)++;
		value_idx++;
	}

}

void	new_str_envp_input(t_token *temp, int *word_rec, \
							char *new_str, char **envp_bundle)
{
	size_t	old_len;
	size_t	idx;
	size_t	n_idx;
	int		envp_idx;

	old_len = ft_strlen(temp->str);
	idx = 0;
	n_idx = 0;
	envp_idx = 0;
	while (idx < old_len)
	{
		if (word_rec[idx] == '$')
		{
			idx++;
			exit_status_input(word_rec, new_str, &n_idx, &idx);
			envp_value_input(envp_bundle, &envp_idx, new_str, &n_idx);
			idx_shift(1, word_rec, &n_idx, &idx);
		}
		else
		{
			n_idx++;
			idx++;
		}
	}
}



void	new_str_literal_input(t_token *temp, int *word_rec, \
								char *new_str, size_t old_len)
{
	size_t	idx;
	size_t	n_idx;
	int		detect;
	char	quotes_type;

	idx = 0;
	n_idx = 0;
	detect = 0;
	quotes_type = 9;
	while (idx < old_len)
	{
		check_quotes(word_rec[idx], &quotes_type, &detect);
		if (quotes_type == word_rec[idx])
			idx++;
		else if (word_rec[idx] == '$')
		{
			idx++;
			idx_shift(0, word_rec, &n_idx, &idx);
		}
		else
			new_str[n_idx++] = temp->str[idx++];
	}
}

size_t	envp_len(int *word_rec, char **envp_bundle, size_t *idx, int *envp_idx)
{
	size_t	envp_len;

	envp_len = 0;
	if (word_rec[*idx] == '?')
	{
		(*idx)++;
		(*envp_idx)++;
		return (2);
	}
	while (word_rec[*idx] == 0)
		(*idx)++;
	if (envp_bundle[*envp_idx] != NULL)
		envp_len = ft_strlen(envp_bundle[*envp_idx]);
	(*envp_idx)++;
	return (envp_len);
}

int	new_str_len(int *word_rec, char **envp_bundle, size_t old_len, size_t idx)
{
	size_t	new_len;
	int		envp_idx;
	int		detect;
	char	quotes_type;

	new_len = 0;
	detect = 0;
	envp_idx = 0;
	quotes_type = 9;
	while (idx < old_len)
	{
		check_quotes(word_rec[idx], &quotes_type, &detect);
		if (quotes_type == word_rec[idx])
			idx++;
		else if (word_rec[idx] == '$')
		{
			idx++;
			new_len += envp_len(word_rec, envp_bundle, &idx, &envp_idx);
		}
		else
		{
			new_len++;
			idx++;
		}
	}
	return (new_len);
}

void	str_renew(t_token *temp, int *word_rec, char **envp_bundle)
{
	char	*new_str;
	size_t	new_len;
	size_t	old_len;

	old_len = ft_strlen(temp->str);
	new_len = new_str_len(word_rec, envp_bundle, old_len, 0);
	new_str = (char *)malloc(sizeof(char) * (new_len + 1));
	new_str[new_len] = 0;
	new_str_literal_input(temp, word_rec, new_str, old_len);
	new_str_envp_input(temp, word_rec, new_str, envp_bundle);
	free(temp->str);
	temp->str = new_str;

	//최종 str이 null인 노드를 없애야하나?
	//하나의 str에 여러개의 환경변수가 존재하고 확장할때 문제가 많음 ㅠㅠ
}

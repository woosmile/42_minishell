/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_renew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 14:33:24 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/29 22:47:24 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	idx_shift(int mode, char *word_rec, size_t *n_idx, size_t *idx)
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

void	exit_status_input(char *word_rec, char *new_str, \
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

void	new_str_envp_input(t_token *temp, char *word_rec, \
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

void	new_str_literal_input(t_token *temp, char *word_rec, \
								char *new_str, size_t old_len)
{
	size_t		idx;
	size_t		n_idx;
	t_quotes	q;

	idx = 0;
	n_idx = 0;
	q.detect = 0;
	q.type = -1;
	while (idx < old_len)
	{
		check_quotes(word_rec[idx], &q);
		if (q.type == word_rec[idx])
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

size_t	envp_len(char *word_rec, char **envp_bundle, size_t *idx, int *envp_idx)
{
	size_t	envp_len;

	envp_len = 0;
	if (word_rec[*idx] == '?')
		(*idx)++;
	else
	{
		while (word_rec[*idx] == 0)
			(*idx)++;
	}
	if (envp_bundle[*envp_idx] != NULL)
		envp_len = ft_strlen(envp_bundle[*envp_idx]);
	(*envp_idx)++;
	return (envp_len);
}

int	new_str_len(char *word_rec, t_exp *exp_head, size_t idx)
{
	size_t		new_len;
	int			envp_idx;
	t_quotes	q;

	new_len = 0;
	envp_idx = 0;
	q.detect = 0;
	q.type = -1;
	while (word_rec[idx] != -1)
	{
		check_quotes(word_rec[idx], &q);
		if (q.type == word_rec[idx])
			idx++;
		else if (word_rec[idx] == '$')
		{
			idx++;
			new_len += envp_len(word_rec, &idx, &envp_idx);
		}
		else
		{
			new_len++;
			idx++;
		}
	}
	return (new_len);
}

void	str_renew(t_token *temp, t_exp *exp_head, char *word_rec)
{
	char	*new_str;
	size_t	new_len;

	new_len = new_str_len(word_rec, exp_head, 0);
	new_str = (char *)malloc(sizeof(char) * (new_len + 1));
	new_str[new_len] = 0;
	new_str_literal_input(temp, word_rec, new_str);
	new_str_envp_input(temp, word_rec, new_str);
	free(temp->str);
	temp->str = new_str;
}

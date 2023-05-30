/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 14:33:24 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/30 20:44:46 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
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

size_t	exp_len(char *word_rec, size_t *idx, t_exp *exp_head)
{
	size_t	exp_len;

	exp_len = 0;
	while (word_rec[*idx] == 0)
		(*idx)++;
	if (exp_head->div == 0)
		exp_len = ft_strlen(exp_head->str);
	return (exp_len);
}

int	new_str_len(char *word_rec, t_exp *exp_head, size_t *idx)
{
	size_t		new_len;
	t_quotes	q;

	new_len = 0;
	q.detect = 0;
	q.type = -1;
	while (word_rec[*idx] != -1)
	{
		check_quotes(word_rec[*idx], &q);
		if (q.type == word_rec[*idx])
			(*idx)++;
		else if (word_rec[*idx] == '$')
		{
			if (q.detect == 1 && q.type == '\"')
				new_len = new_len + double_quotes_exp();
			else
				normal_exp();
		}
		else
		{
			new_len++;
			(*idx)++;
		}
	}
	return (new_len);
}*/

t_token	*expansion_create_token(t_token *temp, t_exp *exp_head)
{
	t_token	*node;
	
	while (exp_head->str != NULL && exp_head != NULL)
	{
		node = new_token_node(WORD, exp_head->str, temp);
		node->next = temp->next;
		temp->next->prev = node;
		temp->next = node;
		temp = temp->next;	
		exp_head = exp_head->next;
	}
	return (temp);
}

t_token	*expansion_str(t_token *temp, t_exp *exp_head, char *word_rec)
{
	int		w_idx;
	int		s_idx;
	char	*new_str;
	char	*old_temp;
	char	*comb_str;
	char	*str_temp;
	char	*last_str;

	w_idx = 0;
	s_idx = 0;
	comb_str = NULL;
	str_temp = temp->str;
	while (word_rec[w_idx] != -1)
	{
		if (word_rec[w_idx] == '$')
		{
			new_str = ft_substr(str_temp, 0, s_idx);
			old_temp = new_str;
			if (exp_head->str != NULL)
				new_str = ft_strjoin(new_str, exp_head->str);
			free(old_temp);
			if (!comb_str)
				comb_str = new_str;
			else
			{
				old_temp = comb_str;
				comb_str = ft_strjoin(comb_str, new_str);
				free(old_temp);
			}
			exp_head = exp_head->next;
			if (exp_head->str == NULL)
				exp_head = exp_head->next;
			else
			{
				free(temp->str);
				temp->str = comb_str;
				temp = expansion_create_token(temp, exp_head);
			}
			w_idx++;
			while (word_rec[w_idx] == 0)
				w_idx++;
			str_temp = str_temp + w_idx;
			s_idx = 0;
		}
		else
		{
			w_idx++;
			s_idx++;
		}
	}
	last_str = ft_substr(str_temp, 0, s_idx);
	if (*last_str)
	{
		if (!comb_str)
			comb_str = last_str;
		else
		{
			old_temp = comb_str;
			comb_str = ft_strjoin(comb_str, last_str);
			free(old_temp);
			free(last_str);
		}
	}
	free(temp->str);
	temp->str = comb_str;
	return (temp);
}
/*
t_token	*expansion_str(t_token *temp, t_exp *exp_head, char *word_rec)
{
	char		*new_str;
	size_t		new_len;
	size_t		idx;

	new_len = new_str_len(word_rec, exp_head, &idx);
	new_str = (char *)malloc(sizeof(char) * (new_len + 1));
	new_str[new_len] = 0;
	new_str_literal_input(temp, word_rec, new_str);
	new_str_envp_input(temp, word_rec, new_str);
	free(temp->str);
	temp->str = new_str;
}
*/

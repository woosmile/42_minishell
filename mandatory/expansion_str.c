/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 14:33:24 by woosekim          #+#    #+#             */
/*   Updated: 2023/06/01 18:04:38 by woosekim         ###   ########.fr       */
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

// t_token	*expansion_create_token(t_token *temp, t_exp **exp_head, char *str_temp)
t_token	*expansion_create_token(t_token *temp, t_exp **exp_head)
{
	t_token	*node;
	
	while ((*exp_head != NULL && (*exp_head)->str != NULL) || \
			((*exp_head)->str == NULL && (*exp_head)->div == 1 && (*exp_head)->next != NULL))
	// while ((*exp_head && (*exp_head)->str != NULL) || (*exp_head)->div == 1)
			// (*exp_head)->div == 1 || ((*exp_head)->str == NULL && (*exp_head)->next)))
	{
		// if ((*exp_head)->str == NULL)
		// 	node = new_token_node(WORD, str_temp, temp);
		// else
		// 	node = new_token_node(WORD, (*exp_head)->str, temp);
		if ((*exp_head)->str == NULL)
			node = new_token_node(WORD, "", temp);
		else if ((*exp_head)->)
		else
			node = new_token_node(WORD, (*exp_head)->str, temp);
		//node = new_token_node(WORD, (*exp_head)->str, temp);
		if (temp->next)
		{
			node->next = temp->next;
			temp->next->prev = node;
		}
		temp->next = node;
		temp = temp->next;
		if ((*exp_head)->str == NULL)
			(*exp_head)->div = 0;
		else
			(*exp_head) = (*exp_head)->next;
	}
	return (temp);
}

char	*front_str_combine(char *str_temp, int s_idx, char *str_comb, t_exp **exp_head)
{
	char	*str_new;
	char	*old_str;

	str_new = ft_substr(str_temp, 0, s_idx);
	if (((*exp_head)->str != NULL && (*exp_head)->div == 0) || ft_strncmp(str_new, "", 2) == 0)
	{
		old_str = str_new;
		str_new = ft_strjoin(str_new, (*exp_head)->str);
		(*exp_head) = (*exp_head)->next;
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

void	rear_str_combine(t_token *temp, char *str_temp, int s_idx, char *str_comb)
{
	char	*str_new;
	char	*str_old;

	if (s_idx > 0)
	{
		str_new = ft_substr(str_temp, 0, s_idx);
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
	if (temp->str != str_comb)
		free(temp->str);
	temp->str = str_comb;
}

char	*idx_init(char *word_rec, int *w_idx, char *str_temp, int *s_idx)
{
	(*w_idx)++;
	while (word_rec[*w_idx] == 0)
		(*w_idx)++;
	str_temp = str_temp + *w_idx;
	*s_idx = 0;
	return (str_temp);
}

t_token *check_expansion_list(t_token *temp, char *str_temp, t_exp **exp_head, char **str_comb)
{
	char	*old_str;
	char	*free_str;
	
	if ((*exp_head)->str == NULL)
		(*exp_head) = (*exp_head)->next;
	else
	{
		free_str = temp->str;
		temp->str = ft_strdup(*str_comb);
		if (!temp->str)
			exit(1);
		free(*str_comb);
		*str_comb = NULL;
		// temp = expansion_create_token(temp, exp_head, str_temp);
		temp = expansion_create_token(temp, exp_head);
		// if ((*exp_head)->div == 0)
		// {
		// 	old_str = temp->str;
		// 	temp->str = ft_strjoin(temp->str, str_temp);
		// 	free(old_str);
		// 	free(free_str);
		// 	(*exp_head) = (*exp_head)->next;
		// }
		old_str = temp->str;
		temp->str = ft_strjoin(temp->str, str_temp);
		free(old_str);
		free(free_str);
		(*exp_head) = (*exp_head)->next;
	}
	return (temp);
}

void	word_rec_recreate(t_token *temp, char **str_temp, char **word_rec, int *w_idx)
{
	*w_idx = 0;
	free(*word_rec);
	*str_temp = temp->str;
	*word_rec = word_type_recorder(temp, 0, 0);
}

t_token	*expansion_str(t_token *temp, char *str_temp, t_exp *exp_head, char *word_rec)
{
	int		w_idx;
	int		s_idx;
	char	*str_comb;

	w_idx = 0;
	s_idx = 0;
	str_comb = NULL;
	while (word_rec[w_idx] != -1)
	{
		if (word_rec[w_idx] == '$')
		{
			str_comb = front_str_combine(str_temp, s_idx, str_comb, &exp_head);
			str_temp = idx_init(word_rec, &w_idx, temp->str, &s_idx);
			temp = check_expansion_list(temp, str_temp, &exp_head, &str_comb);
			if (!str_comb)
				word_rec_recreate(temp, &str_temp, &word_rec, &w_idx);
		}
		else
		{
			w_idx++;
			s_idx++;
		}
	}
	rear_str_combine(temp, str_temp, s_idx, str_comb);
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

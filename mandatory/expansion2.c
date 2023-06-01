/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 16:12:27 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/01 17:43:30 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//iterate str and find env name in list, replace by env_value
char			*exp_str_to_str(char *str, t_env *env_head);
//token->prev > sublst > token->next token value is replaced by sublst_head
t_token			*exp_connect(t_token *token, t_token *sublst_head);
t_token_type	*check_ingredient2(char **split, t_token_type type);
int				find_n_split(char **split);

void	expansion(t_token *token_head, t_env *env_head)
{
	t_token	*token_iter;
	char	*str_with_value;
	t_token	*sublst_head;

	token_iter = *token_head;
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
	if (n_split > 2 && (token->type == INFILE || token->type == OUTFILE
			|| token->type == APPEND))
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

t_token_type	*check_ingredient2(char **split, t_token_type type)
{
	int				len;
	int				i;
	t_token_type	*result;

	len = find_n_split(split);
	result = (t_token_type *)malloc((len + 1) * sizeof (t_token_type));
	if (result == NULL)
		exit (1);
	i = 0;
	while (i < len)
		result[i++] = type;
	result[i] = NULL;
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

	sublst_tail = sublst_head;
	while (sublst_tail->next)
		sublst_tail = sublst_tail->next;
	token->type = sublst_head->type;
	token->str = sublst_head->str;
	next = token->next;
	token->next = sublst_head->next;
	if (sublst_head->next != NULL)
		sublst_head->next->prev = token;
	sublst_tail->next = next;
	if (next != NULL)
		next->prev = sublst_tail;
	return (next);
}

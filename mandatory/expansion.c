/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 16:12:27 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/02 18:19:01 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_token	*exp_head_tail_connect(t_token *token, t_token *sublst_head)
{
	t_token	*sublst_tail;
	t_token	*next;

	sublst_tail = sublst_head;
	while (sublst_tail->next)
		sublst_tail = sublst_tail->next;
	token->type = sublst_head->type;
	free(token->str);
	token->str = sublst_head->str;
	next = token->next;
	token->next = sublst_head->next;
	sublst_head->next->prev = token;
	free(sublst_head);
	sublst_tail->next = next;
	if (next != NULL)
		next->prev = sublst_tail;
	return (next);
}

t_token	*exp_connect(t_token *token, t_token *sublst_head)
{
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
		token->type = sublst_head->type;
		free(sublst_head);
		return (token->next);
	}
	next = exp_head_tail_connect(token, sublst_head);
	return (next);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 14:08:45 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/29 14:22:02 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token_node(t_token_type type, char *split, t_token *prev)
{
	t_token	*node;

	node = (t_token *)malloc(sizeof(t_token));
	if (!node)
		exit(1);
	node->type = type;
	node->str = ft_strdup(split);
	if (!node->str)
		exit(1);
	node->next = NULL;
	node->prev = prev;
	return (node);
}

void	token_list_renew(t_token **token_head, t_token **temp, \
						t_token *temp_prev)
{
	if ((*temp)->prev == *token_head)
	{
		free((*token_head)->str);
		free(*token_head);
		(*token_head) = *temp;
		(*token_head)->prev = NULL;
	}
	else
	{
		(*temp)->prev->prev->next = *temp;
		(*temp)->prev = (*temp)->prev->prev;
		free(temp_prev->str);
		free(temp_prev);
	}
}

int	syntax_error(t_token *token_head, int redir_flag, int pipe_flag, int mode)
{
	if (mode == 0)
		return (error_handling(2));
	else if (mode == 1)
	{
		if (redir_flag == 1)
			return (error_handling(4));
		else if (token_head->next == NULL)
			return (error_handling(5));
	}
	else if (mode == 2)
	{
		if (redir_flag == 1)
			return (error_handling(3));
		else if (pipe_flag == 1)
			return (error_handling(2));
		else if (token_head->next == NULL)
			return (error_handling(6));
	}
	return (0);
}

int	syntax_check(t_token *token_head, t_token_type type, \
				int redir_flag, int pipe_flag)
{
	if (type == PIPE)
		return (syntax_error(token_head, redir_flag, pipe_flag, 0));
	while (token_head)
	{
		type = token_head->type;
		if (type == REDIR)
		{
			if (syntax_error(token_head, redir_flag, pipe_flag, 1) == 1)
				return (1);
			redir_flag = 1;
		}
		else if (type == PIPE)
		{
			if (syntax_error(token_head, redir_flag, pipe_flag, 2) == 1)
				return (1);
			pipe_flag = 1;
		}
		else if (type == WORD)
		{
			redir_flag = 0;
			pipe_flag = 0;
		}
		token_head = token_head->next;
	}
	return (0);
}

void	token_list_free(t_token *token_head)
{
	t_token	*temp;

	temp = token_head;
	while (temp != NULL)
	{
		temp = token_head->next;
		free(token_head->str);
		free(token_head);
		token_head = temp;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 14:27:07 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/19 20:46:16 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type	*check_ingredient(char **split)
{
	t_token_type	*type;
	int				len;
	int				i;

	len = 0;
	while (split[len] != NULL)
		len++;
	type = (t_token_type *)malloc(sizeof(t_token_type) * len);
	i = 0;
	while (split[i] != NULL)
	{
		if (!ft_strncmp(">", split[i], 2) || !ft_strncmp("<", split[i], 2) \
		|| !ft_strncmp(">>", split[i], 3) || !ft_strncmp("<<", split[i], 3))
			type[i] = REDIR;
		else if (!ft_strncmp("|", split[i], 2))
			type[i] = PIPE;
		else
			type[i] = WORD;
		i++;
	}
	return (type);
}

t_token	*new_token_node(t_token_type type, char *split, t_token *prev)
{
	t_token	*node;

	node = (t_token *)malloc(sizeof(t_token));
	node->type = type;
	node->str = ft_strdup(split);
	node->next = NULL;
	node->prev = prev;
	return (node);
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

int	check_syntax(t_token *token_head, t_token_type type, \
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

t_token	*token_list_init(char **split, t_token_type *type, \
						t_token *token_head, t_token *new)
{
	t_token	*temp;
	int		i;

	i = 0;
	while (split[i] != NULL)
	{
		if (token_head == NULL)
		{
			token_head = new_token_node(type[i], split[i], NULL);
			temp = token_head;
		}
		else
		{
			new = new_token_node(type[i], split[i], temp);
			temp->next = new;
			temp = new;
		}
		i++;
	}
	if (check_syntax(token_head, token_head->type, 0, 0))
	{
		token_list_free(token_head);
		return (NULL);
	}
	return (token_head);
}

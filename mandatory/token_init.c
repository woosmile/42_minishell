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

t_token_type	check_redir_type(t_token_type type, t_token *token_head)
{
	if (!ft_strncmp("<", token_head->str, 2))
		type = INFILE;
	else if (!ft_strncmp(">", token_head->str, 2))
		type = OUTFILE;
	else if (!ft_strncmp("<<", token_head->str, 3))
		type = HEREDOC;
	else if (!ft_strncmp(">>", token_head->str, 3))
		type = APPEND;
	return (type);
}

t_token	*token_list_combine(t_token *token_head, t_token *temp)
{
	t_token_type	type;
	int				redir_flag;

	redir_flag = 0;
	while (temp)
	{
		type = temp->type;
		if (type == REDIR)
			redir_flag = 1;
		else if (type == WORD && redir_flag == 1)
		{
			redir_flag = 0;
			type = check_redir_type(type, temp->prev);
			temp->type = type;
			token_list_renew(&token_head, &temp, temp->prev);
		}
		temp = temp->next;
	}
	return (token_head);
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
	if (syntax_check(token_head, token_head->type, 0, 0))
	{
		token_list_free(token_head);
		return (NULL);
	}
	return (token_head);
}
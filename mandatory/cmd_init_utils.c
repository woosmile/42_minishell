/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_init_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 19:01:53 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/23 23:38:26 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*new_cmd_node(t_cmd *prev)
{
	t_cmd	*node;

	node = (t_cmd *)malloc(sizeof(t_cmd));
	node->words = NULL;
	node->words_tail = NULL;
	node->redirs = NULL;
	node->redirs_tail = NULL;
	node->next = NULL;
	node->prev = prev;
	return (node);
}

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

void	rewind_words_redirs_list(t_cmd *cmd_head)
{
	while (cmd_head)
	{
		if (cmd_head->words != NULL)
		{
			while (cmd_head->words->prev != NULL)
				cmd_head->words = cmd_head->words->prev;
		}
		if (cmd_head->redirs != NULL)
		{
			while (cmd_head->redirs->prev != NULL)
				cmd_head->redirs = cmd_head->redirs->prev;
		}
		cmd_head = cmd_head->next;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_init_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 19:01:53 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/30 14:30:47 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*new_cmd_node(t_cmd *prev)
{
	t_cmd	*node;

	node = (t_cmd *)malloc(sizeof(t_cmd));
	if (!node)
		exit(1);
	node->words = NULL;
	node->words_tail = NULL;
	node->redirs = NULL;
	node->redirs_tail = NULL;
	node->next = NULL;
	node->prev = prev;
	return (node);
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

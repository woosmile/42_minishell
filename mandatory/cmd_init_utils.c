/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_init_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 19:01:53 by woosekim          #+#    #+#             */
/*   Updated: 2023/06/02 17:21:50 by woosekim         ###   ########.fr       */
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
	node->prev_pfd[0] = -1;
	node->prev_pfd[1] = -1;
	node->next_pfd[0] = -1;
	node->next_pfd[1] = -1;
	node->cmd_path = NULL;
	node->argv = NULL;
	node->next = NULL;
	node->prev = prev;
	return (node);
}

t_token_type	*check_ingredient(char **split)
{
	t_token_type	*type;
	int				len;
	int				i;

	len = 0;
	while (split[len] != NULL)
		len++;
	type = (t_token_type *)malloc(sizeof(t_token_type) * len);
	if (!type)
		return (NULL);
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

void	cmd_word_list(t_cmd *cmd_temp, t_token *words, \
						t_token **token_head, t_token **token_temp)
{
	if (cmd_temp->words == NULL)
		cmd_temp->words = *token_temp;
	else
	{
		while (words->next != NULL)
			words = words->next;
		words->next = *token_temp;
		cmd_temp->words = cmd_temp->words->next;
	}
	cmd_temp->words_tail = *token_temp;
	if (*token_head == *token_temp)
	{
		(*token_head) = (*token_head)->next;
		if (*token_head != NULL)
			(*token_head)->prev = NULL;
		(*token_temp) = (*token_head);
	}
	else
	{
		(*token_temp)->prev->next = (*token_temp)->next;
		(*token_temp)->next->prev = (*token_temp)->prev;
		(*token_temp) = (*token_temp)->next;
	}
	cmd_temp->words->next = NULL;
	cmd_temp->words->prev = words;
}

void	cmd_redir_list(t_cmd *cmd_temp, t_token *redirs, \
						t_token **token_head, t_token **token_temp)
{
	if (cmd_temp->redirs == NULL)
		cmd_temp->redirs = *token_temp;
	else
	{
		while (redirs->next != NULL)
			redirs = redirs->next;
		redirs->next = *token_temp;
		cmd_temp->redirs = cmd_temp->redirs->next;
	}
	cmd_temp->redirs_tail = *token_temp;
	if (*token_head == *token_temp)
	{
		(*token_head) = (*token_head)->next;
		if (*token_head != NULL)
			(*token_head)->prev = NULL;
		(*token_temp) = (*token_head);
	}
	else
	{
		(*token_temp)->prev->next = (*token_temp)->next;
		(*token_temp)->next->prev = (*token_temp)->prev;
		(*token_temp) = (*token_temp)->next;
	}
	cmd_temp->redirs->next = NULL;
	cmd_temp->redirs->prev = redirs;
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

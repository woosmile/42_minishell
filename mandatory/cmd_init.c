/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 20:08:36 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/19 20:18:17 by woosekim         ###   ########.fr       */
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

t_cmd	*words_redirs_init(t_cmd *cmd_head, t_cmd *cmd_temp, \
						t_token *token_head, t_token *token_temp)
{
	cmd_head = new_cmd_node(NULL);
	cmd_temp = cmd_head;
	while (token_temp)
	{
		if (token_temp->type == WORD)
			cmd_word_list(cmd_temp, cmd_temp->words, &token_head, &token_temp);
		else if (token_temp->type == PIPE)
		{
			token_temp = token_temp->next;
			free(token_head->str);
			free(token_head);
			token_head = token_temp;
			token_head->prev = NULL;
			cmd_temp->next = new_cmd_node(cmd_head);
			cmd_temp = cmd_temp->next;
		}
		else
			cmd_redir_list(cmd_temp, cmd_temp->redirs, \
							&token_head, &token_temp);
	}
	rewind_words_redirs_list(cmd_head);
	return (cmd_head);
}

t_cmd	*cmd_list_init(char *str, t_cmd *cmd_head, t_env *env_head)
{
	char			**split;
	t_token			*token_head;
	t_token_type	*type;

	split = split_str(str, 0, 0, NULL);
	if (!split)
		return (NULL);
	type = check_ingredient(split);
	if (!type)
	{
		free_double_ptr(split);
		return (NULL);
	}
	token_head = token_list_init(split, type, NULL, NULL);
	free_double_ptr(split);
	free(type);
	if (token_head == NULL)
		return (NULL);
	token_head = token_list_combine(token_head, token_head);


	// test_print_tokens(token_head);
	expansion(token_head, env_head);
	g_exit_status = 0;
	// test_print_tokens(token_head);
	unquote(token_head);

	// test_print_cmds(cmd_head);
	cmd_head = parser(token_head, env_head);
	// cmd_head = words_redirs_init(NULL, NULL, token_head, token_head);
	return (cmd_head);
}

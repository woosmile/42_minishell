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

t_cmd	*cmd_list_create(t_token *token_head, t_env *env_head)
{
	t_cmd	*cmd_head;

	token_head = token_list_combine(token_head, token_head);
	expansion(token_head, env_head);
	g_exit_status = 0;
	unquote(token_head);
	cmd_head = words_redirs_init(NULL, NULL, token_head, token_head);
	return (cmd_head);
}

t_cmd	*cmd_list_init(char *str, t_cmd *cmd_head, t_env *env_head)
{
	char			**split;
	t_token			*token_head;
	t_token_type	*type;

	split = split_str(str, 0, 0, 0);
	if (split == NULL)
		return (NULL);
	if (split[0] == NULL)
	{
		free(split);
		return (NULL);
	}
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
	cmd_head = cmd_list_create(token_head, env_head);
	return (cmd_head);
}

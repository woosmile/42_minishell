/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 19:22:54 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/19 19:52:35 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_double_ptr(char **ptr)
{
	int	i;

	if (ptr == NULL)
		return ;
	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free (ptr);
}

void	env_list_free(t_env *head)
{
	t_env	*temp;

	temp = head;
	while (temp != NULL)
	{
		temp = head->next;
		free(head->name);
		free(head->value);
		free(head);
		head = temp;
	}
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

void	cmd_list_free(t_cmd	*cmd_head)
{
	t_cmd	*temp;

	temp = cmd_head;
	while (temp != NULL)
	{
		token_list_free(temp->redirs);
		token_list_free(temp->words);
		temp = cmd_head->next;
		free(cmd_head);
		cmd_head = temp;
	}
}

int	error_handling(int num)
{
	int	result;

	if (num == 1)
		write(2, "Error : unclosed quotation marks.\n", 34);
	else if (num == 2)
		write(2, "Error : invalid pipe.\n", 22);
	else if (num == 3)
		write(2, "Error : pipe immediately after redirection.\n", 44);
	else if (num == 4)
		write(2, "Error : redirection immediately after redirection.\n", 51);
	else if (num == 5)
		write(2, "Error : nothing after redirection.\n", 35);
	else if (num == 6)
		write(2, "Error : nothing after pipe.\n", 28);
	if (num == 0)
		result = 0;
	else
		result = 1;
	return (result);
}

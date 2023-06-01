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


//may need check joonhlee branch to solve compile error
#include "minishell.h"

extern int	g_exit_status;

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

int	error_handling(int select)
{
	if (select == 1)
		write(2, "minishell: syntax error unclosed quotation mark '\n", 50);
	else if (select == 2)
		write(2, "minishell: syntax error unclosed quotation mark \"\n", 50);
	else if (select == 3)
		write(2, "minishell: syntax error near unexpected token `|'\n", 50);
	else if (select == 4)
	{
		write(2, "minishell: ", 11);
		write(2, "syntax error near unexpected token `newline'\n", 45);
	}
	else if (select == 5)
		write(2, "minishell: syntax error near unexpected token `<'\n", 50);
	else if (select == 6)
		write(2, "minishell: syntax error near unexpected token `>'\n", 50);
	else if (select == 7)
		write(2, "minishell: syntax error near unexpected token `<<'\n", 51);
	else if (select == 8)
		write(2, "minishell: syntax error near unexpected token `>>\'\n", 51);
	g_exit_status = 258;
	return (1);
}

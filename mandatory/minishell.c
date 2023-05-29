/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:35:16 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/19 20:18:59 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
void	check_leak(void)
{
	system("leaks --list minishell");
}
*/
int	g_exit_status;

int	main(int argc, char **argv, char **envp)
{
	char	*str;
	t_env	*env_head;
	t_cmd	*cmd_head;

	if (argc != 1)
		return (1);
	(void)argv;
	env_head = env_list_init(envp, NULL, NULL, NULL);
	// TEST ARR TO LIST (ENV)
	// while (env_head != NULL)
	// {
	// 	printf("%s", env_head->name);
	// 	printf("=");
	// 	printf("%s\n", env_head->content);
	// 	env_head = env_head->next;
	// }
	g_exit_status = 123;
	while (1)
	{
		str = readline("$> ");
		cmd_head = cmd_list_init(str, NULL, env_head);
		if (!cmd_head)
			return (1);
		//TEST CMD LIST
		/*
		while (cmd_head)
		{
			while (cmd_head->words)
			{
				printf("%d : %s\n", cmd_head->words->type, cmd_head->words->str);
				cmd_head->words = cmd_head->words->next;
			}
			while (cmd_head->redirs)
			{
				printf("%d : %s\n", cmd_head->redirs->type, cmd_head->redirs->str);
				cmd_head->redirs = cmd_head->redirs->next;
			}
			if (cmd_head->words_tail)
				printf("words tail = %d : %s\n", cmd_head->words_tail->type, cmd_head->words_tail->str);
			if (cmd_head->redirs_tail)
				printf("redirs tail = %d : %s\n", cmd_head->redirs_tail->type, cmd_head->redirs_tail->str);
			cmd_head = cmd_head->next;
			if (cmd_head != NULL)
				printf("\n\nPIPE\n\n");
		}
		*/
		cmd_list_free(cmd_head);
	}

	// TEST LIST TO ARR (ENV)
	// char	**env_arr;
	// env_arr = env_conv_arr(env_head);
	// int	i = 0;
	// while (env_arr[i] != 0)
	// {
	// 	printf("%s\n", env_arr[i]);
	// 	i++;
	// }
	// free_double_ptr(env_arr);

	env_list_free(env_head);
	free(str);
	//atexit(check_leak);
	return (0);
}

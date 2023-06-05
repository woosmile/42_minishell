/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:34:03 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/05 10:30:58 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_env	*env_head;
	char	*line;
	t_cmd	*cmd_head;
	t_here	*here_head;

	if (argc != 1 || argv[1] != 0)
		return (126);
	signal_setup();
	env_head = env_list_init(envp, NULL, NULL, NULL);
	while (1)
	{
		line = readline("minishell> ");
		if (line == 0)
			ctrl_d_handler(env_head, line);
		if (line && *line)
			add_history(line);
		cmd_head = cmd_list_init(line, NULL, env_head);
		here_head = NULL;
		if (g_exit_status == 0)
			here_head = repeat_heredocs(cmd_head, env_head);
		if (g_exit_status == 0)
			g_exit_status = exec_cmds(cmd_head, &env_head);
		clear_this_line(&cmd_head, &here_head, &line);
	}
	return (0);
}

void	clear_this_line(t_cmd **cmd_head, t_here **here_head, char **line)
{
	t_cmd		*cmd_iter;
	t_cmd		*cmd_to_clear;

	cmd_iter = *cmd_head;
	while (cmd_iter)
	{
		cmd_to_clear = cmd_iter;
		cmd_iter = cmd_iter->next;
		token_list_free(cmd_to_clear->words);
		token_list_free(cmd_to_clear->redirs);
		if (cmd_to_clear->cmd_path != NULL)
			free(cmd_to_clear->cmd_path);
		if (cmd_to_clear->argv != NULL)
			free(cmd_to_clear->argv);
		free(cmd_to_clear);
	}
	*cmd_head = NULL;
	*here_head = clear_here_n_return(*here_head);
	free(*line);
	*line = NULL;
}

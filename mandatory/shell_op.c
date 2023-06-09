/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_op.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 08:37:24 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/09 09:05:15 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_cmds(t_cmd *cmd_head, t_env **env_head)
{
	t_cmd	*cmd_iter;
	int		pid;

	cmd_iter = cmd_head;
	while (cmd_iter)
	{
		if (cmd_iter->next != NULL || cmd_iter->prev != NULL)
			if (manage_pipe(cmd_iter) == -1)
				return (perror_return("minishell: pipe", 1));
		if (cmd_iter->prev == NULL && cmd_iter->next == NULL
			&& cmd_iter->words != NULL
			&& is_builtin(cmd_iter->words->str))
			return (only_builtin_child(cmd_iter, env_head));
		pid = fork();
		if (pid == -1)
			return (perror_return("minishell: fork", 1));
		else if (pid == 0)
			return (child(cmd_iter, env_head));
		else if (pid > 0)
			cmd_iter = cmd_iter->next;
	}
	return (parent(pid, cmd_head));
}

int	manage_pipe(t_cmd *cmd)
{
	int	check;

	if (cmd->prev != NULL && cmd->prev->prev_pfd[0] != -1)
	{
		close(cmd->prev->prev_pfd[0]);
		close(cmd->prev->prev_pfd[1]);
		cmd->prev->prev_pfd[0] = -1;
		cmd->prev->prev->next_pfd[0] = -1;
		cmd->prev->prev_pfd[1] = -1;
		cmd->prev->prev->next_pfd[1] = -1;
	}
	if (cmd->next != NULL)
	{
		check = pipe(cmd->next_pfd);
		if (check == -1)
			return (check);
		cmd->next->prev_pfd[0] = cmd->next_pfd[0];
		cmd->next->prev_pfd[1] = cmd->next_pfd[1];
	}
	return (0);
}

int	parent(int pid, t_cmd *cmd_head)
{
	t_cmd	*cmd_iter;
	int		n_cmd;
	int		status;
	int		wpid;

	n_cmd = parent_close_pfd(cmd_head);
	parent_signal_setup();
	cmd_iter = cmd_head;
	while (n_cmd > 0)
	{
		wpid = wait(&status);
		if (wpid == pid && WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == 3)
				ft_putstr_fd("Quit: 3", STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
			g_exit_status = 128 + WTERMSIG(status);
		}
		else if (wpid == pid && WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		n_cmd--;
	}
	signal_setup();
	return (g_exit_status);
}

int	parent_close_pfd(t_cmd *cmd_head)
{
	t_cmd	*cmd_iter;
	int		n_cmd;

	n_cmd = 0;
	cmd_iter = cmd_head;
	while (cmd_iter)
	{
		if (cmd_iter->next_pfd[0] != -1)
		{
			close(cmd_iter->next_pfd[0]);
			close(cmd_iter->next_pfd[1]);
			cmd_iter->next_pfd[0] = -1;
			cmd_iter->next_pfd[1] = -1;
		}
		n_cmd++;
		cmd_iter = cmd_iter->next;
	}
	return (n_cmd);
}

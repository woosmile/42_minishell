/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_op2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 08:37:24 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/02 18:19:18 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	only_builtin_child(t_cmd *cmd, t_env **env_head)
{
	int		std_fd[2];
	t_token	*token_iter;
	int		fd;

	if (obc_init(cmd, std_fd) != 0)
		return (1);
	token_iter = cmd->redirs;
	fd = 0;
	while (token_iter)
	{
		if (obc_redirs(token_iter, std_fd) != 0)
			return (1);
		token_iter = token_iter->next;
	}
	run_only_builtin(cmd, env_head);
	dup2(std_fd[0], STDIN_FILENO);
	close(std_fd[0]);
	dup2(std_fd[1], STDOUT_FILENO);
	close(std_fd[1]);
	return (0);
}

int	obc_init(t_cmd *cmd, int *std_fd)
{
	cmd->argv = words_lst_to_arr(cmd);
	cmd->cmd_path = ft_strdup(cmd->argv[0]);
	if (cmd->cmd_path == NULL)
		exit(EXIT_FAILURE);
	std_fd[0] = dup(STDIN_FILENO);
	if (std_fd[0] == -1)
		return (perror_return("minishell:", 1));
	std_fd[1] = dup(STDOUT_FILENO);
	if (std_fd[1] == -1)
	{
		close(std_fd[0]);
		return (perror_return("minishell:", 1));
	}
	return (0);
}

int	obc_redirs(t_token *token_iter, int *std_fd)
{
	int	fd;

	if (token_iter->type == AERROR)
		return (obc_aerror(token_iter, std_fd));
	if (token_iter->type == INFILE)
		fd = open(token_iter->str, O_RDONLY);
	else if (token_iter->type == OUTFILE)
		fd = open(token_iter->str, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else
		fd = open(token_iter->str, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd == -1)
	{
		dup2(std_fd[0], STDIN_FILENO);
		close(std_fd[0]);
		dup2(std_fd[1], STDOUT_FILENO);
		close(std_fd[1]);
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		return (perror_return(token_iter->str, 1));
	}
	if (token_iter->type == INFILE)
		dup2(fd, STDIN_FILENO);
	else if (token_iter->type == OUTFILE || token_iter->type == APPEND)
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	obc_aerror(t_token *token_iter, int *std_fd)
{
	dup2(std_fd[0], STDIN_FILENO);
	close(std_fd[0]);
	dup2(std_fd[1], STDOUT_FILENO);
	close(std_fd[1]);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(token_iter->str, STDERR_FILENO);
	ft_putstr_fd(" : ambiguous redirect\n", STDERR_FILENO);
	return (1);
}

int	perror_return(char *str, int exit_code)
{
	g_exit_status = exit_code;
	perror(str);
	return (exit_code);
}

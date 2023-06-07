/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_op3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 08:37:24 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/08 07:43:42 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	child(t_cmd *cmd, t_env **env_head)
{
	char	**envp;

	child_init(cmd);
	child_redirs(cmd);
	cmd->argv = words_lst_to_arr(cmd);
	if (cmd->argv[0] == NULL)
		exit (0);
	if (is_builtin(cmd->argv[0]))
		return (run_builtin(cmd, env_head));
	envp = env_conv_arr(*env_head);
	if (envp == 0)
		exit (perror_return("malloc", 1));
	cmd->cmd_path = find_cmd_path(cmd->argv[0], envp);
	child_signal_setup();
	if (execve(cmd->cmd_path, cmd->argv, envp) == -1)
	{
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->cmd_path, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit (127);
	}
	exit (0);
}

void	child_init(t_cmd *cmd)
{
	if (cmd->prev != NULL)
	{
		close(cmd->prev_pfd[1]);
		dup2(cmd->prev_pfd[0], STDIN_FILENO);
		close(cmd->prev_pfd[0]);
	}
	if (cmd->next != NULL)
	{
		close(cmd->next_pfd[0]);
		dup2(cmd->next_pfd[1], STDOUT_FILENO);
		close(cmd->next_pfd[1]);
	}
}

void	child_redirs(t_cmd *cmd)
{
	t_token	*token_iter;
	int		fd;

	token_iter = cmd->redirs;
	while (token_iter)
	{
		fd = child_redirs_open(token_iter);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			exit(perror_return(token_iter->str, 1));
		}
		if (token_iter->type == INFILE)
			dup2(fd, STDIN_FILENO);
		else if (token_iter->type == OUTFILE || token_iter->type == APPEND)
			dup2(fd, STDOUT_FILENO);
		close(fd);
		token_iter = token_iter->next;
	}
}

int	child_redirs_open(t_token *token_iter)
{
	int	fd;

	fd = -1;
	if (token_iter->type == AERROR)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(token_iter->str, STDERR_FILENO);
		ft_putstr_fd(" : ambiguous redirect\n", STDERR_FILENO);
		exit(1);
	}
	else if (token_iter->type == INFILE)
		fd = open(token_iter->str, O_RDONLY);
	else if (token_iter->type == OUTFILE)
		fd = open(token_iter->str, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else if (token_iter->type == APPEND)
		fd = open(token_iter->str, O_WRONLY | O_APPEND | O_CREAT, 0644);
	return (fd);
}

char	**words_lst_to_arr(t_cmd *cmd)
{
	t_token	*token_iter;
	int		n;
	char	**argv;
	int		i;

	token_iter = cmd->words;
	n = 0;
	while (token_iter)
	{
		n++;
		token_iter = token_iter->next;
	}
	argv = (char **)malloc(sizeof(char *) * (n + 1));
	if (argv == NULL)
		exit (EXIT_FAILURE);
	token_iter = cmd->words;
	i = 0;
	while (i < n)
	{
		argv[i] = token_iter->str;
		token_iter = token_iter->next;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

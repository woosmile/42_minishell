/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:10:43 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/02 19:51:53 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (cmd == NULL)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (2);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (3);
	else if (ft_strcmp(cmd, "export") == 0)
		return (4);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (5);
	else if (ft_strcmp(cmd, "env") == 0)
		return (6);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (7);
	else
		return (0);
}

int	run_builtin(t_cmd *cmd, t_env **env_head)
{
	int	check;

	check = 0;
	cmd->cmd_path = ft_strdup(cmd->argv[0]);
	if (ft_strcmp(cmd->cmd_path, "echo") == 0)
		check = ft_echo(cmd->argv);
	else if (ft_strcmp(cmd->cmd_path, "cd") == 0)
		check = ft_cd(cmd, *env_head);
	else if (ft_strcmp(cmd->cmd_path, "pwd") == 0)
		check = ft_pwd();
	else if (ft_strcmp(cmd->cmd_path, "export") == 0)
		check = ft_export(cmd, *env_head);
	else if (ft_strcmp(cmd->cmd_path, "unset") == 0)
		check = ft_unset(cmd, env_head);
	else if (ft_strcmp(cmd->cmd_path, "env") == 0)
		check = ft_env(*env_head);
	else if (ft_strcmp(cmd->cmd_path, "exit") == 0)
		check = ft_exit(cmd, 0);
	exit (check);
}

int	run_only_builtin(t_cmd *cmd, t_env **env_head)
{
	int	check;

	check = 0;
	if (ft_strcmp(cmd->cmd_path, "echo") == 0)
		check = ft_echo(cmd->argv);
	else if (ft_strcmp(cmd->cmd_path, "cd") == 0)
		check = ft_cd(cmd, *env_head);
	else if (ft_strcmp(cmd->cmd_path, "pwd") == 0)
		check = ft_pwd();
	else if (ft_strcmp(cmd->cmd_path, "export") == 0)
		check = ft_export(cmd, *env_head);
	else if (ft_strcmp(cmd->cmd_path, "unset") == 0)
		check = ft_unset(cmd, env_head);
	else if (ft_strcmp(cmd->cmd_path, "env") == 0)
		check = ft_env(*env_head);
	else if (ft_strcmp(cmd->cmd_path, "exit") == 0)
		check = ft_exit(cmd, 0);
	g_exit_status = check;
	return (check);
}

int	ft_echo(char **argv)
{
	int	i;
	int	j;
	int	n_option;

	i = 1;
	n_option = 0;
	echo_check(argv, &i, &j, &n_option);
	while (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (argv[i + 1] != NULL)
			ft_putchar_fd(' ', STDOUT_FILENO);
		else if (argv[i + 1] == NULL && n_option == 0)
			ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
	return (0);
}

void	echo_check(char **argv, int *i, int *j, int *n)
{
	while (argv[*i] && ft_strncmp(argv[*i], "-n", 2) == 0)
	{
		*n = *i;
		*j = 2;
		while (argv[*i][*j])
		{
			if (argv[*i][*j] != 'n')
			{
				*n -= 1;
				break ;
			}
			*j += 1;
		}
		if (*n != *i)
			break ;
		*i += 1;
	}
}

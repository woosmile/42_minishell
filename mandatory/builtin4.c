/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:10:43 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/02 18:25:48 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(t_cmd *cmd, int exit_code)
{
	int		i;
	int		n;
	char	*a;

	i = 1;
	if (cmd->argv[i] == NULL)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		exit(exit_code);
	}
	n = ft_atoi(cmd->argv[1]);
	a = ft_itoa(n);
	if (a == NULL)
		exit (EXIT_FAILURE);
	if (ft_strcmp(cmd->argv[1], a) != 0)
		exit_numeric_error(cmd->argv[1], a);
	else
		return (exit_too_many_error(cmd->argv[2], a, n));
	return (0);
}

void	exit_numeric_error(char *arg, char *a)
{
	ft_putstr_fd("exit\n", STDERR_FILENO);
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	free(a);
	exit(255);
}

int	exit_too_many_error(char *arg, char *a, int n)
{
	free(a);
	if (arg != NULL)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	else
		exit(n % 256);
}

int	is_valid_name(char *str)
{
	int	i;

	if (str[0] == '\0' || !(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

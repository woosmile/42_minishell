/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:10:43 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/09 10:35:56 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(t_cmd *cmd, int exit_code)
{
	int		i;
	int		n;

	if (cmd->prev == NULL && cmd->next == NULL)
		ft_putstr_fd("exit\n", STDERR_FILENO);
	i = 1;
	if (cmd->argv[i] == NULL)
	{
		exit(g_exit_status);
	}
	n = ft_exit_atoi(cmd->argv[1], &i);
	if (i != 1)
		exit_numeric_error(cmd->argv[1]);
	else
		return (exit_too_many_error(cmd->argv[2], n));
	return (exit_code);
}

int	ft_exit_atoi(const char *str, int *check)
{
	int		i;
	int		sign;
	long	nbr;

	i = 0;
	sign = 1;
	nbr = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '+' || str[i] == '-')
		sign = 44 - str[i++];
	while (str[i] && *check == 1)
	{
		if (str[i] < '0' || str[i] > '9')
			*check = 0;
		else if (sign > 0 && ((nbr > LONG_MAX / 10)
				|| (nbr == LONG_MAX / 10 && str[i] - '0' > LONG_MAX % 10)))
			*check = 0;
		else if (sign < 0 && ((nbr < LONG_MIN / 10) || (nbr == LONG_MIN / 10
					&& sign * (str[i] - '0') < LONG_MIN % 10)))
			*check = 0;
		nbr = nbr * 10 + sign * (str[i++] - '0');
	}
	return ((int)nbr);
}

void	exit_numeric_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	exit(255);
}

int	exit_too_many_error(char *arg, int n)
{
	if (arg != NULL)
	{
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

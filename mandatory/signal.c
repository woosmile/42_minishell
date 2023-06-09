/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:34:03 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/09 10:15:00 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_setup(void)
{
	t_termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	ctrl_c_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_exit_status = 1;
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
	}
	else
		return ;
}

void	ctrl_d_handler(t_env *env_head, char *line)
{
	ft_putstr_fd("\033[1A", STDOUT_FILENO);
	ft_putstr_fd("\033[11C", STDOUT_FILENO);
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	env_list_free(env_head);
	free(line);
	exit(0);
}

void	here_signal_setup(void)
{
	signal(SIGINT, here_ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	here_ctrl_c_handler(int signum)
{
	if (signum != SIGINT)
		return ;
	ft_putstr_fd("\n", STDOUT_FILENO);
	close(STDIN_FILENO);
	g_exit_status = -1;
}

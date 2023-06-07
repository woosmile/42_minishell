/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 10:16:12 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/07 16:59:06 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parent_signal_setup(void)
{
	t_termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	signal(SIGINT, parent_ctrl_c_handler);
	signal(SIGQUIT, parent_ctrl_slash_handler);
}

void	parent_ctrl_c_handler(int signum)
{
	if (signum != SIGINT)
		return ;
	ft_putstr_fd("\n", STDOUT_FILENO);
}

void	parent_ctrl_slash_handler(int signum)
{
	if (signum != SIGQUIT)
		return ;
	ft_putstr_fd("Quit: 3\n", STDOUT_FILENO);
}

void	child_signal_setup(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

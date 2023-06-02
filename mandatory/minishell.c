/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:34:03 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/02 10:12:17 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_env				*env_head;
	char				*line;

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
		shell_op(line, &env_head);
		free(line);
	}
	return (0);
}

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
	ft_putstr_fd("exit", STDOUT_FILENO);
	env_list_free(env_head);
	free(line);
	exit(0);
}

void	here_signal_setup(void)
{
	// t_termios	term;
	// // struct sigaction act;

	// tcgetattr(STDIN_FILENO, &term);
	// term.c_lflag &= ~ECHOCTL;
	// tcsetattr(STDIN_FILENO, TCSANOW, &term);
	// ft_bzero(&act, sizeof(act));
	// act.__sigaction_u.__sa_handler = &here_ctrl_c_handler;
	// sigaction(SIGINT, &act, NULL);
	signal(SIGINT, here_ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	here_ctrl_c_handler(int signum)
{
	// int	std_fd;
	// int	pfd[2];
	// int	check;

	if (signum != SIGINT)
		return ;
	// std_fd = dup(STDIN_FILENO);
	// if (std_fd == -1)
	// {
	// 	perror("minishell:");
	// 	return ;
	// }
	// check = pipe(pfd);
	// if (check == -1)
	// {
	// 	close(std_fd);
	// 	perror("minishell");
	// 	return ;
	// }
	// dup2(pfd[0], STDIN_FILENO);
	// close(pfd[0]);
	// rl_done = 1;
	// printf("here handler 127\n");
	if (g_exit_status == 0)
	{
		ft_putstr_fd("\n(press enter)", STDOUT_FILENO);
		// rl_on_new_line();
		rl_replace_line("", 1);
	}

	g_exit_status = -1;
	// close(STDIN_FILENO);
	// dup2(std_fd, STDIN_FILENO);
	// close(std_fd);
	// rl_replace_line("", 1);
	// rl_redisplay();
	// ft_putstr_fd("\033[K", STDOUT_FILENO);
	// ft_putchar_fd('\n', pfd[1]);
	// ft_putchar_fd('\0', pfd[1]);
	// close(pfd[1]);
	// dup2(std_fd, STDIN_FILENO);
	// close(std_fd);
}

void	test_print_env(t_env *env_head)
{
	t_env	*env_iter;

	env_iter = env_head;
	while (env_iter)
	{
		printf("env|key=%s|value=%s\n", env_iter->name, env_iter->value);
		env_iter = env_iter->next;
	}
}

void	test_print_tokens(t_token *token_head)
{
	t_token	*token_iter;

	token_iter = token_head;
	while (token_iter)
	{
		printf("token_type=%d|token_value=%s|prev=%p|current=%p|next=%p\n",
			token_iter->type, token_iter->str,
			token_iter->prev, token_iter, token_iter->next);
		token_iter = token_iter->next;
	}
}

void test_print_cmds(t_cmd *cmd_head)
{
	t_cmd *cmd_iter;
	t_token	*token_iter;

	cmd_iter = cmd_head;
	while (cmd_iter)
	{
		token_iter = cmd_iter->words;
		while (token_iter)
		{
			printf("token_type=%d|token_value=%s|prev=%p|current=%p|next=%p\n",
				token_iter->type, token_iter->str,
				token_iter->prev, token_iter, token_iter->next);
			token_iter = token_iter->next;
		}
		token_iter = cmd_iter->redirs;
		while (token_iter)
		{
			printf("token_type=%d|token_value=%s|prev=%p|current=%p|next=%p\n",
				token_iter->type, token_iter->str,
				token_iter->prev, token_iter, token_iter->next);
			token_iter = token_iter->next;
		}
		cmd_iter = cmd_iter->next;
	}
}

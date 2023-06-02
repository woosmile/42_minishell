/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:35:13 by woosekim          #+#    #+#             */
/*   Updated: 2023/06/02 18:17:54 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define READLINE_LIBRARY

# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <signal.h>
# include <limits.h>
# include <termios.h>
# include "libft.h"
# include "readline.h"
# include "history.h"

typedef struct termios	t_termios;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
	struct s_env	*prev;
}					t_env;

typedef enum e_token_type
{
	WORD = 0,
	PIPE,
	REDIR,
	INFILE,
	OUTFILE,
	HEREDOC,
	APPEND,
	AERROR,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*str;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_cmd
{
	t_token			*words;
	t_token			*words_tail;
	t_token			*redirs;
	t_token			*redirs_tail;
	int				prev_pfd[2];
	int				next_pfd[2];
	char			*cmd_path;
	char			**argv;
	struct s_cmd	*prev;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_here
{
	char			*filename;
	struct s_here	*next;
}					t_here;

typedef struct s_quotes
{
	int		detect;
	char	type;
}			t_quotes;

typedef struct s_exp
{
	char			*str;
	int				div;
	struct s_exp	*next;
}					t_exp;

// int	exit_status;
int g_exit_status;

int error_handling(int select);

t_token *exp_connect(t_token *token, t_token *sublst_head);
t_token_type *check_ingredient2(char **split, t_token_type type);
int find_n_split(char **split);

t_token *tokenize_str(char *str_w_value, t_token *token);

t_env *env_list_init(char **envp, t_env *env_head,
					 t_env *temp, t_env *new);
char **env_conv_arr(t_env *env_head);

void env_list_free(t_env *head);
void free_double_ptr(char **ptr);
void token_list_free(t_token *token_head);
void cmd_list_free(t_cmd *cmd_head);

char **split_str(char *s, size_t str_cnt, size_t s_len,
				 char **str_arr);
int split_null_guard(char **str_arr, int i);
int blank_masking(char *s, size_t s_len, char *word_rec);
int check_sep(char const s, char *charset);
void check_quotes(char s, t_quotes *q);
void check_metacharacter(const char *s, int *i,
						 size_t *cnt, char *word_rec);
// int split_null_guard(char **str_arr, int i);

t_token *token_list_init(char **split, t_token_type *type,
						 t_token *temp, t_token *new);
t_token *new_token_node(t_token_type type, char *split, t_token *prev);
int syntax_check(t_token *token_head, t_token_type type,
				 int redir_flag, int pipe_flag);
t_token *token_list_combine(t_token *token_head, t_token *temp);
void token_list_renew(t_token **token_head, t_token **temp,
					  t_token *prev_temp);

t_cmd *cmd_list_init(char *str, t_cmd *cmd_head, t_env *env_head);
t_cmd *new_cmd_node(t_cmd *prev);
void rewind_words_redirs_list(t_cmd *cmd_head);

void expansion(t_token *token_head, t_env *env_head);
// char *env_get_value(t_env *env_head, char *name); // builtin.c
char **env_arr(char *str, t_env *env_head, char *word_rec);
t_exp *exp_list(t_token *temp, t_exp *exp_head,
				char *word_rec, char **exp_bundle);
t_exp *new_exp_node(char *str);
void exp_list_add_back(t_exp **exp_head, t_exp *new);
void exp_list_add_split(t_exp **exp_head, t_exp *new,
						char **exp_split, char *exp_bundle);
void free_exp_list(t_exp *exp_head);
char *word_type_recorder(char *str, size_t str_len, size_t i);
t_token *expansion_str(t_token *temp, char *str_temp, t_exp *exp_head, char *word_rec);
void check_div_null_node(t_exp *exp_head);
char *exp_str_to_str(char *str, t_env *env_head);

t_env	*new_env_node(char *name, char *value, t_env *prev);
void	split_name_value(char *envp, char **name, char **value);
char *env_find_name(char *envp);
char *env_find_value(char *envp);

void signal_setup(void);
void ctrl_c_handler(int signum);
void ctrl_d_handler(t_env *env_head, char *line);
void here_signal_setup(void);
void here_ctrl_c_handler(int signum);
void clear_this_line(t_cmd *cmd_head, t_here *here_head, char *line);

int exec_cmds(t_cmd *cmd_head, t_env **env_head);
int manage_pipe(t_cmd *cmd);
int only_builtin_child(t_cmd *cmd, t_env **env_head);
int obc_init(t_cmd *cmd, int *std_fd);
int obc_redirs(t_token *token_iter, int *std_fd);
int obc_aerror(t_token *token_iter, int *std_fd);
int child(t_cmd *cmd, t_env **env_head);
void child_init(t_cmd *cmd);
void child_redirs(t_cmd *cmd);
int child_redirs_open(t_token *token_iter);
char **words_lst_to_arr(t_cmd *cmd);
char *find_cmd_path(char *cmd, char **envp);
char **find_path_env(char **envp);
char *check_cmd_path(char *cmd, char **dirs);
int parent(int pid, t_cmd *cmd_head);
int parent_close_pfd(t_cmd *cmd_head);
int perror_return(char *str, int exit_code);

void merge_redir(t_token **token_head);
t_token_type convert_type(char *str);

t_cmd *parser(t_token *token_head, t_env *env_head);
t_cmd *new_cmd_node2(void);

t_here *repeat_heredocs(t_cmd *cmd_head, t_env *env_head);
t_here *here_iter(t_cmd *cmd_head, t_env *env_head);
t_here *do_a_heredoc(char *limiter, t_env *env_head);
char *nexist_name(void);
void write_heredoc(int fd, char *limiter, t_env *env_head);
t_here *free_n_return(char *str, int exit_code);
t_here *clear_here_n_return(t_here *here_head);
void here_add_bottom(t_here **here_head, t_here *here_doc);
void update_redirs(t_token *redirs, t_here *here_doc);


// char **env_conv_arr(t_env *env_head);
// void list_free(t_env *head);
void arr2d_free(char **arr);

int is_builtin(char *cmd);
int run_builtin(t_cmd *cmd, t_env **env_head);
int run_only_builtin(t_cmd *cmd, t_env **env_head);
int ft_echo(char **argv);
void echo_check(char **argv, int *i, int *j, int *n);
int ft_cd(t_cmd *cmd, t_env *env_head);
char *get_dest_path(t_cmd *cmd, t_env *env_head);
char *put_str_return(char *str, int fd);
int perror_free_return(char *first, char *second, int code);
int ft_pwd(void);
int ft_export(t_cmd *cmd, t_env *env_head);
int export_no_arg(t_env *env_head);
int export_syntax_error(char *name);
int ft_unset(t_cmd *cmd, t_env **env_head);
int ft_env(t_env *env_head);
int ft_exit(t_cmd *cmd, int exit_code);
void exit_numeric_error(char *arg, char *a);
int exit_too_many_error(char *arg, char *a, int n);
int is_valid_name(char *str);
int	env_set_value(t_env *env_head, char *name, char *value);
char *env_set_new_value(char *value);
char *env_get_value(t_env *env_head, char *name);
int env_remove(t_env **env_head, char *name);
t_env *env_find_env_to_remove(t_env **env_head, char *name);

int unquote(t_token *token_head);
char *unquote_a_str(char *str);
int find_n_of_quotes(char *str);
int check_quotes2(char s, t_quotes *q);

void test_print_env(t_env *env_head);
void test_print_tokens(t_token *token_head);
void test_print_cmds(t_cmd *cmd_head);

#endif

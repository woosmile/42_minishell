/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:35:13 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/19 20:14:39 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define READLINE_LIBRARY

# include <stdio.h>
# include "../libft/libft.h"
# include "../readline/include/readline/readline.h"
# include "../readline/include/readline/history.h"

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

typedef enum e_token_type
{
	WORD = 0,
	PIPE,
	REDIR,		// '<' '>' '<<' '>>'
				// convert from REDIR + WORD into one of followings
	INFILE, 	// content "infile" meaning "< infile"
	OUTFILE,	// content "outfile" meaning "> outfile"
	HEREDOC,	// content "limiter" meaning "<< limiter"
	APPEND,		// content "outfile2 meaning ">> outfile2"
	AERROR,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;  //0 : word, 1 : pipe|, 2 : redir < > << >>
	char			*str;  //split result
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_cmd
{
	t_token			*words;		// list of word-type tokens
	t_token			*words_tail;
	t_token			*redirs;	// list of redirs-type tokens
	t_token			*redirs_tail;
	int				prev_pfd[2]; // pipes to communicate with prev command
	int				next_pfd[2]; // pipes to communicate with next command
	char			*cmd_path;  // first argv as command name
	char			**argv;     // convert from token list words
	struct s_cmd	*prev;		// to check if it's the first commmand
	struct s_cmd	*next;		// to check if it's the first commmand
}					t_cmd;

// typedef struct s_redir
// {
// 	t_redir_type	r_type;  // 0 : infile<, 1 : outfile>, 2 : heredoc<<, 3 : append>>
// 	char			*file_name;  //type next token
// }	t_redir;

// typedef struct s_ast
// {

// 	t_redir			*redir;
// 	t_cmd			*cmd;
// 	struct s_ast	*left;
// 	struct s_ast	*right;  //left == NULL && right == NULL (->end node)
// }	t_ast;

typedef struct s_quotes
{
	int		detect;
	char	type;
}	t_quotes;

typedef struct s_exp
{
	char			*str;
	int				div;
	struct s_exp	*next;
}	t_exp;

int				error_handling(int select);

t_env			*env_list_init(char **envp, t_env *env_head, \
								t_env *temp, t_env *new);
char			**env_conv_arr(t_env *env_head);

void			env_list_free(t_env *head);
void			free_double_ptr(char **ptr);
void			token_list_free(t_token *token_head);
void			cmd_list_free(t_cmd	*cmd_head);

char			**split_str(char *s, size_t str_cnt, size_t s_len, \
							char **str_arr);
int				split_null_guard(char **str_arr, int i);
int				blank_masking(char *s, size_t s_len, char *word_rec);
int				check_sep(char const s, char *charset);
void			check_quotes(char s, t_quotes *q);
void			check_metacharacter(const char *s, int *i, \
									size_t *cnt, char *word_rec);
int				split_null_guard(char **str_arr, int i);

t_token			*token_list_init(char **split, t_token_type *type, \
								t_token *temp, t_token *new);
t_token			*new_token_node(t_token_type type, char *split, t_token *prev);
int				syntax_check(t_token *token_head, t_token_type type, \
							int redir_flag, int pipe_flag);
t_token			*token_list_combine(t_token *token_head, t_token *temp);
void			token_list_renew(t_token **token_head, t_token **temp, \
								t_token *prev_temp);

t_cmd			*cmd_list_init(char *str, t_cmd *cmd_head, t_env *env_head);
t_cmd			*new_cmd_node(t_cmd *prev);
void			rewind_words_redirs_list(t_cmd *cmd_head);

void			expansion(t_token *token_head, t_env *env_head);
char			*env_get_value(t_env *env_head, char *name);   //builtin.c
char			**env_arr(t_env *env_head, t_token *temp, char *word_rec);
t_exp			*exp_list(t_token *temp, t_exp *exp_head, \
							char *word_rec, char **exp_bundle);
t_exp			*new_exp_node(char *str);
void			exp_list_add_back(t_exp **exp_head, t_exp *new);
void			exp_list_add_split(t_exp **exp_head, t_exp *new, \
									char **exp_split, char *exp_bundle);
void			free_exp_list(t_exp *exp_head);
t_token			*expansion_str(t_token *temp, char *str_temp, t_exp *exp_head, char *word_rec);

#endif

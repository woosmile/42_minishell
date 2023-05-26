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

t_env			*env_list_init(char **envp, t_env *env_head, \
								t_env *temp, t_env *new);
char			**env_conv_arr(t_env *env_head);

int				error_handling(int num);
void			env_list_free(t_env *head);
void			free_double_ptr(char **ptr);
void			token_list_free(t_token *token_head);
void			cmd_list_free(t_cmd	*cmd_head);

char			**split_str(char *s);
int				blank_masking(char *s, size_t s_len, int *word_rec);
int				check_sep(char const s, char *charset);
void			check_quotes(char s, char *quotes_type, int *detect);
void			check_metacharacter(const char *s, int *i, \
									size_t *cnt, int *word_rec);
int				split_null_guard(char **str_arr, int i);

t_token			*token_list_init(char **split, t_token_type *type, \
								t_token *temp, t_token *new);
t_token_type	*check_ingredient(char **split);

t_cmd			*cmd_list_init(char *str, t_cmd *cmd_head, t_env *env_head);
t_cmd			*new_cmd_node(t_cmd *prev);
t_token_type	check_redir_type(t_token_type type, t_token *token_head);
void			token_list_renew(t_token **token_head, t_token **temp, \
								t_token *prev_temp);
void			rewind_words_redirs_list(t_cmd *cmd_head);

void			expansion_main(t_cmd *cmd_head, t_env *env_head);
char			*env_get_value(t_env *env_head, char *name);
void			envp_value(t_env *env_head, char **envp_bundle, \
							int envp_arr_cnt);
void			envp_name(t_token *temp, int *word_rec, \
							char **envp_bundle, size_t str_len);
void			envp_name_input(t_token *temp, char *envp_name, \
								int name_len, size_t *w_idx);
void			str_renew(t_token *temp, int *word_rec, char **envp_bundle);

#endif

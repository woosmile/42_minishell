/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_list_init.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 21:14:51 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/30 16:40:43 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern	int	g_exit_status;

int	check_ambiguous(t_token *temp, char **exp_split)
{
	size_t	i;

	i = 0;
	while (exp_split[i])
		i++;
	if (i > 1)
	{
		if (temp->type == INFILE || temp->type == OUTFILE || \
			temp->type == APPEND)
		{
			temp->type = AERROR;
			write(2, "minishell: ", 11);
			write(2, temp->str, ft_strlen(temp->str));
			write(2, ": ambiguous redirect\n", 21);
			g_exit_status = 1;
			return (1);
		}
	}
	return (0);
}

void	exp_list_init(t_token *temp, t_exp **exp_head, \
						char *exp_bundle, t_quotes q)
{
	char	**exp_split;
	t_exp	*new;

	if ((q.detect == 1 && q.type == '\"') || exp_bundle == NULL)
	{
		new = new_exp_node(exp_bundle);
		new->div = 0;
		exp_list_add_back(exp_head, new);
	}
	else
	{
		exp_split = ft_split(exp_bundle, ' ');
		if (!exp_split)
			exit(1);
		if (check_ambiguous(temp, exp_split))
			return ;
		exp_list_add_split(exp_head, NULL, exp_split, exp_bundle);
		free_double_ptr(exp_split);
	}
	new = new_exp_node(NULL);
	exp_list_add_back(exp_head, new);
}

t_exp	*exp_list(t_token *temp, t_exp *exp_head, \
				char *word_rec, char **exp_bundle)
{
	size_t		w_idx;
	size_t		b_idx;
	t_quotes	q;

	w_idx = 0;
	b_idx = 0;
	q.detect = 0;
	while (word_rec[w_idx] != -1)
	{
		check_quotes(word_rec[w_idx], &q);
		if (word_rec[w_idx] == '$')
		{
			exp_list_init(temp, &exp_head, exp_bundle[b_idx], q);
			b_idx++;
		}
		w_idx++;
	}
	return (exp_head);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 02:02:12 by woosekim          #+#    #+#             */
/*   Updated: 2023/06/01 14:54:04 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	word_type_recording(t_token *temp, char *word_rec, \
							size_t i, int exp_flag)
{
	static int		dollar_flag;

	if (temp->str[i] == '$' && exp_flag == 1 && dollar_flag == 0)
	{
		word_rec[i] = temp->str[i];
		dollar_flag = 1;
	}
	else if ((temp->str[i] == '$' && exp_flag == 1) || \
			temp->str[i] == '\"' || temp->str[i] == '\'' || \
			temp->str[i] == '>' || temp->str[i] == '<' || \
			temp->str[i] == '|' || temp->str[i] == ' ' || \
			temp->str[i] == '?')
	{
		word_rec[i] = temp->str[i];
		dollar_flag = 0;
	}
	if (i == ft_strlen(temp->str) - 1)
		dollar_flag = 0;
}

char	*word_type_recorder(t_token *temp, size_t str_len, size_t i)
{
	char		*word_rec;
	int			exp_flag;
	t_quotes	q;

	str_len = ft_strlen(temp->str);
	word_rec = (char *)ft_calloc(str_len + 1, sizeof(char));
	if (!word_rec)
		exit(1);
	q.detect = 0;
	while (i < str_len)
	{
		exp_flag = 1;
		check_quotes(temp->str[i], &q);
		if ((q.detect == 1 && q.type == '\'') || temp->type == HEREDOC)
			exp_flag = 0;
		word_type_recording(temp, word_rec, i, exp_flag);
		i++;
	}
	word_rec[i] = -1;
	return (word_rec);
}

void	expansion(t_token *token_head, t_env *env_head)
{
	char	*word_rec;
	char	**exp_bundle;
	t_exp	*exp_head;

	while (token_head)
	{
		word_rec = word_type_recorder(token_head, 0, 0);
		exp_bundle = env_arr(env_head, token_head, word_rec);
		//word_rec TEST
		/*
		size_t x_0;
		x_0 = 0;
		while (x_0 < ft_strlen(token_head->str))
		{
			printf("%d ", word_rec[x_0]);
			x_0++;
		}
		printf("\n");
		*/
		//exp_bundle TEST
		/*
		int	x;
		x = 0;
		while (x < exp_arr_cnt)
		{
			if (exp_bundle[x] != NULL)
				printf("%s\n", exp_bundle[x]);
			else if (exp_bundle[x] == NULL) 
				printf("(null)\n");
			x++;
		}
		*/
		exp_head = exp_list(token_head, NULL, word_rec, exp_bundle);
		//exp list TEST
		/*
		while (exp_head)
		{
			if (exp_head->str == NULL)
				printf("(null)\n");
			else
				printf("%s\n", exp_head->str);
			exp_head = exp_head->next;
		}
		*/
		token_head = expansion_str(token_head, token_head->str, \
									exp_head, word_rec);
		free_exp_list(exp_head);
		free_double_ptr(exp_bundle);
		free(word_rec);
		token_head = token_head->next;
	}	
}

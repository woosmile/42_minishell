/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 02:02:12 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/25 20:28:25 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**envp_arr(t_env *env_head, t_token *temp, \
					int *word_rec, int *envp_arr_cnt)
{
	size_t	str_len;
	char	**envp_bundle;
	size_t	i;

	str_len = ft_strlen(temp->str);
	i = 0;
	while (i < str_len)
	{
		if (word_rec[i] == '$')
			(*envp_arr_cnt)++;
		i++;
	}
	if (*envp_arr_cnt == 0)
		return (NULL);
	envp_bundle = (char **)malloc(sizeof(char *) * (*envp_arr_cnt + 1));
	envp_bundle[*envp_arr_cnt] = NULL;
	envp_name(temp, word_rec, envp_bundle, str_len);
	envp_value(env_head, envp_bundle, *envp_arr_cnt);
	return (envp_bundle);
}

void	word_type_recording(t_token *temp, int *word_rec, \
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

int	*word_type_recorder(t_token *temp, size_t str_len, size_t i)
{
	int		*word_rec;
	int		exp_flag;
	int		detect;
	char	quotes_type;

	str_len = ft_strlen(temp->str);
	word_rec = (int *)ft_calloc(str_len, sizeof(int));
	detect = 0;
	while (i < str_len)
	{
		exp_flag = 1;
		check_quotes(temp->str[i], &quotes_type, &detect);
		if ((detect == 1 && quotes_type == '\'') || temp->type == HEREDOC)
			exp_flag = 0;
		word_type_recording(temp, word_rec, i, exp_flag);
		i++;
	}
	return (word_rec);
}

void	expansion_str(t_token *temp, t_env *env_head)
{
	int		*word_rec;
	char	**envp_bundle;
	int		envp_arr_cnt;

	while (temp)
	{
		envp_arr_cnt = 0;
		word_rec = word_type_recorder(temp, 0, 0);
		envp_bundle = envp_arr(env_head, temp, word_rec, &envp_arr_cnt);
		
		//word_rec TEST
		/*
		size_t x_0;
		x_0 = 0;
		while (x_0 < ft_strlen(temp->str))
		{
			printf("%d ", word_rec[x_0]);
			x_0++;
		}
		//envp_bundle TEST
		int	x;
		x = 0;
		while (x < envp_arr_cnt)
		{
			if (envp_bundle[x] != NULL)
				printf("%s\n", envp_bundle[x]);
			else if (envp_bundle[x] == NULL)
				printf("(null)\n");
			x++;
		}
		*/
		str_renew(temp, word_rec, envp_bundle);
		free(word_rec);
		if (envp_bundle != NULL)
			free_double_ptr(envp_bundle);
		temp = temp->next;
	}	
}

void	expansion_main(t_cmd *cmd_head, t_env *env_head)
{
	while (cmd_head)
	{
		expansion_str(cmd_head->words, env_head);
		expansion_str(cmd_head->redirs, env_head);
		cmd_head = cmd_head->next;
	}
}

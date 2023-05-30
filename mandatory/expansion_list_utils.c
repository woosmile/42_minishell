/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_list_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 21:17:14 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/30 16:29:20 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exp	*new_exp_node(char *str)
{
	t_exp	*node;

	node = (t_exp *)malloc(sizeof(t_exp));
	if (!node)
		exit(1);
	if (str == NULL)
		node->str = str;
	else
	{
		node->str = ft_strdup(str);
		if (!node->str)
			exit(1);
	}
	node->div = 0;
	node->next = NULL;
	return (node);
}

void	exp_list_add_back(t_exp **exp_head, t_exp *new)
{
	t_exp	*head_temp;

	head_temp = *exp_head;
	if (new != 0)
	{
		if (*exp_head == 0)
			*exp_head = new;
		else
		{
			while ((*exp_head)->next != 0)
				*exp_head = (*exp_head)->next;
			(*exp_head)->next = new;
			*exp_head = head_temp;
		}
	}
}

void	exp_list_add_split(t_exp **exp_head, t_exp *new, \
							char **exp_split, char *exp_bundle)
{
	size_t	i;
	size_t	bundle_len;

	i = 0;
	bundle_len = 0;
	while (exp_split[i])
	{
		new = new_exp_node(exp_split[i]);
		if (i == 0 && exp_bundle[0] == ' ')
			new->div = 1;
		else if (exp_split[i + 1] == NULL)
		{
			bundle_len = ft_strlen(exp_bundle) - 1;
			if (exp_bundle[bundle_len] == ' ')
				new->div = 1;
		}
		exp_list_add_back(exp_head, new);
		i++;
	}
}

void	free_exp_list(t_exp *exp_head)
{
	t_exp	*temp;

	temp = exp_head;
	while (temp != NULL)
	{
		if (temp->str)
			free(temp->str);
		temp = exp_head->next;
		free(exp_head);
		exp_head = temp;
	}
}
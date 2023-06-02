/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 13:32:16 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/02 18:21:42 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_here	*free_n_return(char *str, int exit_code)
{
	free(str);
	g_exit_status = exit_code;
	return (NULL);
}

t_here	*clear_here_n_return(t_here *here_head)
{
	t_here	*iter;
	t_here	*node_to_clear;

	if (here_head == NULL)
		return (NULL);
	iter = here_head;
	while (iter)
	{
		node_to_clear = iter;
		iter = iter->next;
		unlink(node_to_clear->filename);
		free(node_to_clear->filename);
		free(node_to_clear);
	}
	return (NULL);
}

void	here_add_bottom(t_here **here_head, t_here *here_doc)
{
	t_here	*iter;

	if (*here_head == NULL)
	{
		*here_head = here_doc;
		return ;
	}
	iter = *here_head;
	while (iter->next)
		iter = iter->next;
	iter->next = here_doc;
}

void	update_redirs(t_token *redirs, t_here *here_doc)
{
	free(redirs->str);
	redirs->str = ft_strdup(here_doc->filename);
	if (redirs->str == NULL)
		exit(EXIT_FAILURE);
	redirs->type = INFILE;
}

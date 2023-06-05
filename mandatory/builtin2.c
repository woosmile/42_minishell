/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:10:43 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/05 10:43:21 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(t_cmd *cmd, t_env *env_head)
{
	int		check;
	char	*dest_path;
	char	*old_path;
	char	current_path[PATH_MAX];

	if (cmd->argv[1] != NULL && ft_strlen(cmd->argv[1]) == 0)
		return (0);
	dest_path = get_dest_path(cmd, env_head);
	if (dest_path == NULL)
		return (1);
	check = chdir(dest_path);
	if (check == 0 && cmd->argv[1] && ft_strcmp(cmd->argv[1], "-") == 0)
		ft_pwd();
	if (check == -1)
		return (perror_free_return("minishell: cd: ", dest_path, 1));
	else
	{
		getcwd(current_path, PATH_MAX);
		old_path = env_get_value(env_head, "PWD");
		env_set_value(env_head, "OLDPWD", old_path);
		free(old_path);
		env_set_value(env_head, "PWD", current_path);
		free(dest_path);
		return (0);
	}
}

char	*get_dest_path(t_cmd *cmd, t_env *env_head)
{
	char	*dest_path;

	if (cmd->argv[1] == NULL)
	{
		dest_path = env_get_value(env_head, "HOME");
		if (dest_path == NULL)
			return (put_str_return("minishell: cd: HOME not set\n", 2));
	}
	else if (ft_strcmp(cmd->argv[1], "-") == 0)
	{
		dest_path = env_get_value(env_head, "OLDPWD");
		if (dest_path == NULL)
			return (put_str_return("minishell: cd: OLDPWD not set\n", 2));
	}
	else
	{
		dest_path = ft_strdup(cmd->argv[1]);
		if (dest_path == NULL)
			exit(EXIT_FAILURE);
	}
	return (dest_path);
}

char	*put_str_return(char *str, int fd)
{
	ft_putstr_fd(str, fd);
	return (NULL);
}

int	perror_free_return(char *first, char *second, int code)
{
	ft_putstr_fd(first, STDERR_FILENO);
	perror(second);
	free(second);
	return (code);
}

int	ft_pwd(void)
{
	char	current_path[PATH_MAX];
	char	*result;

	result = getcwd(current_path, PATH_MAX);
	if (result == NULL)
	{
		perror("minishell:");
		return (1);
	}
	ft_putendl_fd(current_path, STDOUT_FILENO);
	return (0);
}

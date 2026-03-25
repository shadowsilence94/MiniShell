/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 20:35:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/15 00:05:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_identifier(char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i])
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	ft_export(char **args, char ***envp)
{
	int		i;
	char	*key;
	char	*eq;

	if (!args[1])
		return (print_export(*envp), 0);
	i = 0;
	while (args[++i])
	{
		eq = ft_strchr(args[i], '=');
		key = ft_substr(args[i], 0, (eq - args[i]) * (eq != NULL)
				+ ft_strlen(args[i]) * (eq == NULL));
		if (!is_valid_identifier(key))
			ft_putendl_fd("minishell: export: invalid identifier", 2);
		else if (eq)
			set_env(envp, key, eq + 1);
		free(key);
	}
	return (0);
}

int	ft_env(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		if (ft_strchr(envp[i], '='))
			ft_putendl_fd(envp[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}

static void	remove_var(char ***envp, int j)
{
	free((*envp)[j]);
	while ((*envp)[j])
	{
		(*envp)[j] = (*envp)[j + 1];
		j++;
	}
}

int	ft_unset(char **args, char ***envp)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	while (args[++i])
	{
		if (!is_valid_identifier(args[i]))
			ft_putendl_fd("minishell: unset: invalid identifier", 2);
		else
		{
			j = -1;
			len = ft_strlen(args[i]);
			while ((*envp)[++j])
			{
				if (ft_strncmp((*envp)[j], args[i], len) == 0
					&& (*envp)[j][len] == '=')
				{
					remove_var(envp, j);
					break ;
				}
			}
		}
	}
	return (0);
}

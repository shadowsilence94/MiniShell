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

static int	is_v(char *s)
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

static int	unvalid_id(char *b, char *id, char *key)
{
	if (is_v(key))
		return (0);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(b, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd(id, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	return (1);
}

int	ft_export(char **args, char ***env)
{
	int		i;
	int		l;
	char	*key;
	char	*eq;
	int		status;

	if (!args[1])
		return (print_export(*env), 0);
	i = 0;
	status = 0;
	while (args[++i])
	{
		eq = ft_strchr(args[i], '=');
		l = ft_strlen(args[i]);
		if (eq)
			l = eq - args[i];
		key = ft_substr(args[i], 0, l);
		if (unvalid_id("export", args[i], key))
			status = 1;
		else if (eq)
			set_env(env, key, eq + 1);
		free(key);
	}
	return (status);
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

int	ft_unset(char **args, char ***env)
{
	int	i;
	int	j;
	int	l;
	int	s;

	i = 0;
	s = 0;
	while (args[++i])
	{
		if (unvalid_id("unset", args[i], args[i]))
			s = 1;
		else
		{
			j = 0;
			l = ft_strlen(args[i]);
			while ((*env)[j])
			{
				if (!ft_strncmp((*env)[j], args[i], l) && (*env)[j][l] == '=')
					remove_var(env, j);
				else
					j++;
			}
		}
	}
	return (s);
}

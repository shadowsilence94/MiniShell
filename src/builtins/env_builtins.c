/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-14 20:35:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026-03-14 21:10:00 by hko-ko           ###   ########.fr       */
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

static int	export_one(char *arg, char ***envp)
{
	char	*key;
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (equal)
		key = ft_substr(arg, 0, equal - arg);
	else
		key = ft_strdup(arg);
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		free(key);
		return (1);
	}
	if (equal)
		set_env(envp, key, equal + 1);
	free(key);
	return (0);
}

int	ft_export(char **args, char ***envp)
{
	int		i;
	int		ret;

	if (!args[1])
		return (print_export(*envp), 0);
	i = 1;
	ret = 0;
	while (args[i])
	{
		if (export_one(args[i], envp))
			ret = 1;
		i++;
	}
	return (ret);
}

static void	unset_var(char ***envp, char *key)
{
	int	j;
	int	k_len;

	j = 0;
	k_len = ft_strlen(key);
	while ((*envp)[j])
	{
		if (ft_strncmp((*envp)[j], key, k_len) == 0
			&& (*envp)[j][k_len] == '=')
		{
			free((*envp)[j]);
			while ((*envp)[j])
			{
				(*envp)[j] = (*envp)[j + 1];
				j++;
			}
			return ;
		}
		j++;
	}
}

int	ft_unset(char **args, char ***envp)
{
	int		i;
	int		ret;

	if (!args[1])
		return (0);
	i = 1;
	ret = 0;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			ret = 1;
		}
		else
			unset_var(envp, args[i]);
		i++;
	}
	return (ret);
}

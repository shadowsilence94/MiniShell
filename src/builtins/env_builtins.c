/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-14 20:35:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026-03-14 20:35:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export(char **args, char ***envp)
{
	int		i;
	char	*key;
	char	*value;
	char	*equal;

	if (!args[1])
		return (ft_env(*envp));
	i = 1;
	while (args[i])
	{
		equal = ft_strchr(args[i], '=');
		if (equal)
		{
			key = ft_substr(args[i], 0, equal - args[i]);
			value = ft_strdup(equal + 1);
			set_env(envp, key, value);
			free(key);
			free(value);
		}
		i++;
	}
	return (0);
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

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		unset_var(envp, args[i]);
		i++;
	}
	return (0);
}

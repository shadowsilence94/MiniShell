/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-14 19:20:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026-03-14 22:25:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_env(char **envp)
{
	char	**new_env;
	int		i;
	int		count;

	count = 0;
	while (envp[count])
		count++;
	new_env = (char **)malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(envp[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

char	*get_env_value(char **envp, char *key)
{
	int	i;
	int	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
			return (envp[i] + key_len + 1);
		i++;
	}
	return (NULL);
}

static int	update_env(char **env, char *key, char *val, int l)
{
	int	i;

	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp(env[i], key, l) && env[i][l] == '=')
		{
			free(env[i]);
			env[i] = val;
			return (1);
		}
	}
	return (0);
}

void	set_env(char ***envp, char *key, char *value)
{
	int		i;
	char	**new;
	char	*s;
	char	*t;

	t = ft_strjoin(key, "=");
	s = ft_strjoin(t, value);
	free(t);
	if (update_env(*envp, key, s, ft_strlen(key)))
		return ;
	i = 0;
	while ((*envp)[i])
		i++;
	new = (char **)malloc(sizeof(char *) * (i + 2));
	ft_memcpy(new, *envp, sizeof(char *) * i);
	new[i] = s;
	new[i + 1] = NULL;
	free(*envp);
	*envp = new;
}

void	remove_var(char ***envp, int j)
{
	free((*envp)[j]);
	while ((*envp)[j])
	{
		(*envp)[j] = (*envp)[j + 1];
		j++;
	}
}

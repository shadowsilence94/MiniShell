/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-14 19:20:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026-03-14 19:20:00 by hko-ko           ###   ########.fr       */
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

static char	*create_env_str(char *key, char *value)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(key, "=");
	res = ft_strjoin(tmp, value);
	free(tmp);
	return (res);
}

void	set_env(char ***envp, char *key, char *value)
{
	int		i;
	int		key_len;
	char	**new_env;
	char	*new_str;

	i = 0;
	key_len = ft_strlen(key);
	new_str = create_env_str(key, value);
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], key, key_len) == 0
			&& (*envp)[i][key_len] == '=')
		{
			free((*envp)[i]);
			(*envp)[i] = new_str;
			return ;
		}
		i++;
	}
	new_env = (char **)malloc(sizeof(char *) * (i + 2));
	ft_memcpy(new_env, *envp, sizeof(char *) * i);
	new_env[i] = new_str;
	new_env[i + 1] = NULL;
	free(*envp);
	*envp = new_env;
}

void	increment_shlvl(char ***envp)
{
	char	*shlvl;
	int		val;
	char	*new_val;

	shlvl = get_env_value(*envp, "SHLVL");
	if (!shlvl)
		set_env(envp, "SHLVL", "1");
	else
	{
		val = ft_atoi(shlvl) + 1;
		new_val = ft_itoa(val);
		set_env(envp, "SHLVL", new_val);
		free(new_val);
	}
}

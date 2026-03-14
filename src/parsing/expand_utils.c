/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2025/12/26 12:00:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_var_name(char *val, int *i)
{
	int		start;
	char	*name;

	start = ++(*i);
	if (val[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	while (val[*i] && (ft_isalnum(val[*i]) || val[*i] == '_'))
		(*i)++;
	name = ft_substr(val, start, *i - start);
	return (name);
}

static char	*get_var_value(char *name, char **envp, int last_status)
{
	char	*val;

	if (ft_strncmp(name, "?", 2) == 0)
		return (ft_itoa(last_status));
	val = get_env_value(envp, name);
	if (val)
		return (ft_strdup(val));
	return (ft_strdup(""));
}

char	*expand_status(char *val, int last_status, char **envp)
{
	char	*res;
	char	*name;
	char	*var_val;
	char	*tmp;
	int		i;

	res = ft_strdup("");
	i = 0;
	while (val[i])
	{
		if (val[i] == '$' && val[i+1])
		{
			name = get_var_name(val, &i);
			var_val = get_var_value(name, envp, last_status);
			tmp = ft_strjoin(res, var_val);
			free(res);
			res = tmp;
			free(name);
			free(var_val);
		}
		else
		{
			tmp = (char *)malloc(2);
			tmp[0] = val[i++];
			tmp[1] = '\0';
			char *new_res = ft_strjoin(res, tmp);
			free(res);
			res = new_res;
			free(tmp);
		}
	}
	return (res);
}

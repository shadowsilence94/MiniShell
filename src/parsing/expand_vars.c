/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/15 12:00:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_value(char *name, char **envp, int last_status)
{
	char	*val;

	if (ft_strncmp(name, "?", 2) == 0)
		return (ft_itoa(last_status));
	val = get_env_value(envp, name);
	if (val)
		return (ft_strdup(val));
	return (ft_strdup(""));
}

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

char	*handle_expansion(char *val, int *i, t_exec_params *p, bool dq)
{
	char	*name;
	char	*var_val;
	int		j;

	name = get_var_name(val, i);
	var_val = get_var_value(name, *p->envp, *p->last_status);
	free(name);
	if (!var_val)
		var_val = ft_strdup("");
	if (!dq)
	{
		j = -1;
		while (var_val[++j])
		{
			if (var_val[j] == '*')
				var_val[j] = '\2';
		}
	}
	return (var_val);
}

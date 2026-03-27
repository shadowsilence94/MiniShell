/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/14 22:30:00 by hko-ko           ###   ########.fr       */
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

static char	*handle_expansion(char *val, int *i, t_exec_params *p, bool dq)
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
			if (var_val[j] == '*')
				var_val[j] = '\2';
	}
	return (var_val);
}

char	*expand_status(char *val, t_exec_params *params)
{
	char	*res;
	int		i;
	bool	q[3];

	res = ft_strdup("");
	i = 0;
	ft_memset(q, 0, 3 * sizeof(bool));
	while (val && val[i])
	{
		if ((val[i] == '\'' && !q[1]) || (val[i] == '"' && !q[0]))
			handle_status_quotes(val[i], q, &i);
		else if (params && is_expandable(val, i, q[0]))
			res = apply_expansion(res, handle_expansion(val, &i, params, q[1]));
		else if (val[i] == '*' && !q[0] && !q[1])
		{
			res = append_char(res, '\2');
			i++;
		}
		else
			res = append_char(res, val[i++]);
	}
	if (res[0] == '\0' && !q[2])
		return (free(res), ft_strdup("\1"));
	return (res);
}

char	*expand_heredoc_line(char *line, t_exec_params *params)
{
	char	*res;
	int		i;

	res = ft_strdup("");
	i = 0;
	while (line && line[i])
	{
		if (is_expandable(line, i, false))
			res = apply_expansion(res, handle_expansion(line, &i, params,
						true));
		else
			res = append_char(res, line[i++]);
	}
	return (res);
}

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

static char	*handle_expansion(char *res, char *val, int *i, t_exec_params *p)
{
	char	*name;
	char	*var_val;
	char	*tmp;

	name = get_var_name(val, i);
	var_val = get_var_value(name, *p->envp, *p->last_status);
	tmp = ft_strjoin(res, var_val);
	free(res);
	free(name);
	free(var_val);
	return (tmp);
}

static void	toggle_quotes(char c, bool *s_quote, bool *d_quote)
{
	if (c == '\'' && !*d_quote)
		*s_quote = !*s_quote;
	else if (c == '"' && !*s_quote)
		*d_quote = !*d_quote;
}

static bool	is_expandable(char *v, int i, bool sq)
{
	if (v[i] != '$' || sq || !v[i + 1])
		return (false);
	if (ft_isalnum(v[i + 1]) || v[i + 1] == '_' || v[i + 1] == '?')
		return (true);
	return (false);
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
		{
			q[2] = true;
			toggle_quotes(val[i++], &q[0], &q[1]);
		}
		else if (is_expandable(val, i, q[0]))
			res = handle_expansion(res, val, &i, params);
		else
			res = append_char(res, val[i++]);
	}
	if (res[0] == '\0' && !q[2])
	{
		free(res);
		return (NULL);
	}
	return (res);
}

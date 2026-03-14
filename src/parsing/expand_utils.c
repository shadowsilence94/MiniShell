/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/14 21:10:00 by hko-ko           ###   ########.fr       */
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

static char	*append_char(char *res, char c)
{
	char	tmp[2];
	char	*new_res;

	tmp[0] = c;
	tmp[1] = '\0';
	new_res = ft_strjoin(res, tmp);
	free(res);
	return (new_res);
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

char	*expand_status(char *val, t_exec_params *params)
{
	char	*res;
	int		i;
	bool	s_quote;
	bool	d_quote;

	res = ft_strdup("");
	i = 0;
	s_quote = false;
	d_quote = false;
	while (val[i])
	{
		if ((val[i] == '\'' && !d_quote) || (val[i] == '"' && !s_quote))
		{
			toggle_quotes(val[i++], &s_quote, &d_quote);
			continue ;
		}
		if (val[i] == '$' && !s_quote && val[i + 1] && \
			(ft_isalnum(val[i + 1]) || val[i + 1] == '_' || val[i + 1] == '?'))
			res = handle_expansion(res, val, &i, params);
		else
			res = append_char(res, val[i++]);
	}
	return (res);
}

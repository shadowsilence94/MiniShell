/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/15 12:00:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_special(char *val, int *i, char **res, t_exec_params *p)
{
	bool	st[3];

	if (val[*i] == '\\' && !p->sq && ((p->dq && (val[*i + 1] == '"'
					|| val[*i + 1] == '$' || val[*i + 1] == '\\'))
			|| (!p->dq && val[*i + 1])))
	{
		*res = append_char(*res, val[++(*i)]);
		(*i)++;
		return (1);
	}
	if ((val[*i] == '\'' && !p->dq) || (val[*i] == '"' && !p->sq))
	{
		st[0] = p->sq;
		st[1] = p->dq;
		handle_status_quotes(val[*i], st, i);
		p->sq = st[0];
		p->dq = st[1];
		p->any_q = true;
		return (1);
	}
	return (0);
}

static void	process_char(char *val, int *i, char **res, t_exec_params *p)
{
	if (check_special(val, i, res, p))
		return ;
	if (p && is_expandable(val, *i, p->sq))
	{
		*res = apply_expansion(*res, handle_expansion(val, i, p, p->dq));
		return ;
	}
	else if (val[*i] == '*' && !p->sq && !p->dq)
		*res = append_char(*res, '\2');
	else
		*res = append_char(*res, val[*i]);
	(*i)++;
}

char	*expand_status(char *val, t_exec_params *params)
{
	char	*res;
	int		i;

	res = ft_strdup("");
	i = 0;
	params->sq = false;
	params->dq = false;
	params->any_q = false;
	while (val && val[i])
		process_char(val, &i, &res, params);
	if (res[0] == '\0' && !params->any_q)
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

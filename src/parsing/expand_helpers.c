/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 12:30:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/15 12:30:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	toggle_quotes(char c, bool *s_quote, bool *d_quote)
{
	if (c == '\'' && !*d_quote)
		*s_quote = !*s_quote;
	else if (c == '"' && !*s_quote)
		*d_quote = !*d_quote;
}

char	*apply_expansion(char *res, char *var_val)
{
	char	*tmp;

	tmp = ft_strjoin(res, var_val);
	free(res);
	free(var_val);
	return (tmp);
}

bool	is_expandable(char *v, int i, bool sq)
{
	if (v[i] != '$' || sq || !v[i + 1])
		return (false);
	if (ft_isalnum(v[i + 1]) || v[i + 1] == '_' || v[i + 1] == '?')
		return (true);
	return (false);
}

void	handle_status_quotes(char c, bool q[3], int *i)
{
	q[2] = true;
	toggle_quotes(c, &q[0], &q[1]);
	(*i)++;
}

char	*append_char(char *res, char c)
{
	char	tmp[2];
	char	*new_res;

	tmp[0] = c;
	tmp[1] = '\0';
	new_res = ft_strjoin(res, tmp);
	free(res);
	return (new_res);
}

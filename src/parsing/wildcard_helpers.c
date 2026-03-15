/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 12:30:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/15 12:40:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_unquoted_wildcard(char *str)
{
	int		i;
	bool	q[2];

	i = -1;
	q[0] = false;
	q[1] = false;
	while (str[++i])
	{
		if (str[i] == '\'' && !q[1])
			q[0] = !q[0];
		else if (str[i] == '"' && !q[0])
			q[1] = !q[1];
		else if (str[i] == '*' && !q[0] && !q[1])
			return (1);
	}
	return (0);
}

int	has_unquoted_var(char *str)
{
	int		i;
	bool	q[2];

	i = -1;
	q[0] = false;
	q[1] = false;
	while (str[++i])
	{
		if (str[i] == '\'' && !q[1])
			q[0] = !q[0];
		else if (str[i] == '"' && !q[0])
			q[1] = !q[1];
		else if (str[i] == '$' && !q[0] && str[i + 1])
			return (1);
	}
	return (0);
}

int	get_expanded_count(char *arg)
{
	int		c;
	char	*tmp;
	t_token	*list;
	t_token	*curr;

	c = 0;
	if (ft_strchr(arg, '\2'))
	{
		tmp = ft_strdup(arg);
		while (ft_strchr(tmp, '\2'))
			*ft_strchr(tmp, '\2') = '*';
		list = expand_wildcard(tmp);
		curr = list;
		while (curr && ++c)
			curr = curr->next;
		free_tokens(list);
		free(tmp);
	}
	else
		c = 1;
	return (c);
}

void	clean_marker(char *pattern)
{
	int	i;

	i = -1;
	while (pattern[++i])
		if (pattern[i] == '\2')
			pattern[i] = '*';
}

void	fill_wildcard_list(char *arg, char **new_args, int *c)
{
	char	*tmp;
	t_token	*list;
	t_token	*curr;

	tmp = ft_strdup(arg);
	while (ft_strchr(tmp, '\2'))
		*ft_strchr(tmp, '\2') = '*';
	list = expand_wildcard(tmp);
	curr = list;
	while (curr)
	{
		new_args[(*c)++] = ft_strdup(curr->value);
		curr = curr->next;
	}
	free_tokens(list);
	free(tmp);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 23:30:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/14 23:30:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>
#include <fnmatch.h>

char	*prepare_pattern(char *pattern)
{
	char	*p;
	char	*tmp;

	p = ft_strdup(pattern);
	if (!p)
		return (NULL);
	tmp = p;
	while (*tmp)
	{
		if (*tmp == '\2')
			*tmp = '*';
		tmp++;
	}
	return (p);
}

int	count_wildcard_matches(char *pattern)
{
	DIR				*dir;
	struct dirent	*e;
	int				count;
	char			*p;

	p = prepare_pattern(pattern);
	dir = opendir(".");
	if (!p || !dir)
		return (free(p), 0);
	count = 0;
	e = readdir(dir);
	while (e)
	{
		if (e->d_name[0] != '.' || p[0] == '.')
			if (fnmatch(p, e->d_name, 0) == 0)
				count++;
		e = readdir(dir);
	}
	closedir(dir);
	return (free(p), count);
}

static t_token	*matches_to_tokens(char **matches, int count)
{
	t_token	*head;
	int		i;

	head = NULL;
	sort_strings(matches, count);
	i = -1;
	while (++i < count)
		append_token(&head, new_token(matches[i], TOKEN_WORD));
	return (head);
}

static t_token	*get_final_tokens(char **matches, int count, char *pattern)
{
	if (count == 0)
	{
		clean_marker(pattern);
		return (new_token(ft_strdup(pattern), TOKEN_WORD));
	}
	return (matches_to_tokens(matches, count));
}

t_token	*expand_wildcard(char *pattern)
{
	DIR				*dir;
	char			*matches[1024];
	int				c;
	struct dirent	*e;
	char			*p;

	p = prepare_pattern(pattern);
	dir = opendir(".");
	if (!p || !dir)
		return (free(p), NULL);
	c = 0;
	e = readdir(dir);
	while (e && c < 1024)
	{
		if ((e->d_name[0] != '.' || p[0] == '.')
			&& fnmatch(p, e->d_name, 0) == 0)
			matches[c++] = ft_strdup(e->d_name);
		e = readdir(dir);
	}
	closedir(dir);
	free(p);
	return (get_final_tokens(matches, c, pattern));
}

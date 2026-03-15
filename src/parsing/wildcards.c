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

void	sort_strings(char **arr, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strncmp(arr[i], arr[j], 1024) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

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

int	count_wildcard_matches(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;
	char			*p;

	count = 0;
	p = ft_strdup(pattern);
	while (ft_strchr(p, '\2'))
		*ft_strchr(p, '\2') = '*';
	dir = opendir(".");
	if (!dir)
		return (free(p), 0);
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' || p[0] == '.')
			if (fnmatch(p, entry->d_name, 0) == 0)
				count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (free(p), count);
}

t_token	*expand_wildcard(char *pattern)
{
	DIR				*dir;
	t_token			*head;
	char			*matches[1024];
	int				c;
	int				i;
	struct dirent	*e;
	char			*p;

	c = 0;
	head = NULL;
	p = ft_strdup(pattern);
	while (ft_strchr(p, '\2'))
		*ft_strchr(p, '\2') = '*';
	dir = opendir(".");
	if (!dir)
		return (free(p), NULL);
	e = readdir(dir);
	while (e && c < 1024)
	{
		if (e->d_name[0] != '.' || p[0] == '.')
			if (fnmatch(p, e->d_name, 0) == 0)
				matches[c++] = ft_strdup(e->d_name);
		e = readdir(dir);
	}
	closedir(dir);
	if (c == 0)
	{
		i = -1;
		while (pattern[++i])
			if (pattern[i] == '\2')
				pattern[i] = '*';
		return (free(p), new_token(ft_strdup(pattern), TOKEN_WORD));
	}
	sort_strings(matches, c);
	i = -1;
	while (++i < c)
		append_token(&head, new_token(matches[i], TOKEN_WORD));
	return (free(p), head);
}

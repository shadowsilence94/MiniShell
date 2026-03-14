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

static void	sort_strings(char **arr, int count)
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
	bool	s_q;
	bool	d_q;

	i = 0;
	s_q = false;
	d_q = false;
	while (str[i])
	{
		if (str[i] == '\'' && !d_q)
			s_q = !s_q;
		else if (str[i] == '"' && !s_q)
			d_q = !d_q;
		else if (str[i] == '*' && !s_q && !d_q)
			return (1);
		i++;
	}
	return (0);
}

t_token	*expand_wildcard(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	t_token			*head;
	char			*matches[1024];
	int				count;
	int				i;

	count = 0;
	head = NULL;
	dir = opendir(".");
	if (!dir)
		return (NULL);
	while ((entry = readdir(dir)) && count < 1024)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
			continue ;
		if (fnmatch(pattern, entry->d_name, 0) == 0)
			matches[count++] = ft_strdup(entry->d_name);
	}
	closedir(dir);
	if (count == 0)
		return (new_token(ft_strdup(pattern), TOKEN_WORD));
	sort_strings(matches, count);
	i = 0;
	while (i < count)
		append_token(&head, new_token(matches[i++], TOKEN_WORD));
	return (head);
}

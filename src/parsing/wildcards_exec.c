/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/15 12:00:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_expanded(char **args)
{
	int		i;
	int		c;
	char	*tmp;
	t_token	*list;
	t_token	*curr;

	i = -1;
	c = 0;
	while (args[++i])
	{
		if (ft_strchr(args[i], '\2'))
		{
			tmp = ft_strdup(args[i]);
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
			c++;
	}
	return (c);
}

static void	fill_expanded(char **args, char **new_args)
{
	int		i;
	int		c;
	char	*tmp;
	t_token	*list;
	t_token	*curr;

	i = -1;
	c = 0;
	while (args[++i])
	{
		if (ft_strchr(args[i], '\2'))
		{
			tmp = ft_strdup(args[i]);
			while (ft_strchr(tmp, '\2'))
				*ft_strchr(tmp, '\2') = '*';
			list = expand_wildcard(tmp);
			curr = list;
			while (curr)
			{
				new_args[c++] = ft_strdup(curr->value);
				curr = curr->next;
			}
			free_tokens(list);
			free(tmp);
		}
		else
			new_args[c++] = ft_strdup(args[i]);
	}
	new_args[c] = NULL;
}

static char	**get_expanded_args(char **args)
{
	int		count;
	char	**new_args;

	if (!args)
		return (NULL);
	count = count_expanded(args);
	new_args = (char **)malloc(sizeof(char *) * (count + 1));
	if (!new_args)
		return (NULL);
	fill_expanded(args, new_args);
	return (new_args);
}

void	expand_cmd_wildcards(t_command *cmd)
{
	char	**new_args;
	t_redir	*r;
	t_token	*list;
	int		i;

	if (cmd->args)
	{
		new_args = get_expanded_args(cmd->args);
		i = -1;
		while (cmd->args[++i])
			free(cmd->args[i]);
		free(cmd->args);
		cmd->args = new_args;
	}
	r = cmd->redirs;
	while (r)
	{
		if (r->filename && ft_strchr(r->filename, '\2'))
		{
			if (count_wildcard_matches(r->filename) > 1)
			{
				free(r->filename);
				r->filename = ft_strdup("");
			}
			else
			{
				list = expand_wildcard(r->filename);
				if (list)
				{
					free(r->filename);
					r->filename = ft_strdup(list->value);
					free_tokens(list);
				}
			}
		}
		r = r->next;
	}
	if (cmd->sub_cmd)
		expand_cmd_wildcards(cmd->sub_cmd);
	if (cmd->next)
		expand_cmd_wildcards(cmd->next);
}

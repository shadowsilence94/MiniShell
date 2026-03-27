/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/14 21:10:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_word_end(char *line, int i)
{
	char	quote;

	while (line[i] && !is_whitespace(line[i])
		&& !ft_strchr("|<>&();", line[i]))
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			quote = line[i++];
			while (line[i] && (line[i] != quote))
			{
				if (line[i] == '\\' && quote == '"' && line[i + 1] == quote)
					i += 2;
				else
					i++;
			}
			if (!line[i])
				return (-1);
			i++;
		}
		else if (line[i] == '\\' && line[i + 1])
			i += 2;
		else
			i++;
	}
	return (i);
}

static int	last_is_redir(t_token *head)
{
	t_token	*tmp;

	if (!head)
		return (0);
	tmp = head;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp->type >= TOKEN_REDIRECT_IN && tmp->type <= TOKEN_HEREDOC);
}

static void	add_word_token(t_token **head, char *word,
				char *expanded, bool quoted)
{
	if (expanded && last_is_redir(*head))
	{
		if ((ft_strchr(expanded, ' ') && !ft_strchr(word, '\'')
				&& !ft_strchr(word, '"')) || (ft_strchr(expanded, '\2')
				&& count_wildcard_matches(expanded) > 1))
		{
			free(expanded);
			expanded = ft_strdup("");
		}
	}
	if (expanded)
		append_token(head, new_token(expanded, TOKEN_WORD, quoted));
}

int	handle_word(char *line, int i, t_token **head, t_exec_params *params)
{
	int		end;
	char	*word;
	char	*expanded;

	end = get_word_end(line, i);
	if (end == -1)
		return (-1);
	word = ft_substr(line, i, end - i);
	expanded = expand_status(word, params);
	add_word_token(head, word, expanded, params->any_q);
	free(word);
	return (end);
}

int	is_whitespace(char c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}

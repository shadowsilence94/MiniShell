/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antigrav <antigrav@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 17:00:00 by antigrav       #+#    #+#             */
/*   Updated: 2026/03/14 17:00:00 by antigrav      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

int	get_word_end(char *line, int i)
{
	char	quote;
	while (line[i] && !is_whitespace(line[i]) && line[i] != '|'
		&& line[i] != '<' && line[i] != '>')
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			quote = line[i++];
			while (line[i] && line[i] != quote)
				i++;
			if (line[i])
				i++;
		}

		else
			i++;
	}

	return (i);
}

int	handle_word(char *line, int i, t_token **head, int last_status)
{
	int		start;
	int		end;
	char	*val;
	start = i;
	end = get_word_end(line, i);
	val = ft_substr(line, start, end - start);
	val = expand_status(val, last_status);
	append_token(head, new_token(val, TOKEN_WORD));
	return (end);
}


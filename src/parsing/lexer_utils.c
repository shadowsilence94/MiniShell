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

	while (line[i] && !is_whitespace(line[i]) && line[i] != '|'
		&& line[i] != '<' && line[i] != '>')
	{
		if (line[i] == '\'' || line[i] == '"')
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

int	handle_word(char *line, int i, t_token **head, t_exec_params *params)
{
	int		end;
	char	*word;
	char	*expanded;

	end = get_word_end(line, i);
	word = ft_substr(line, i, end - i);
	expanded = expand_status(word, params);
	append_token(head, new_token(expanded, TOKEN_WORD));
	free(word);
	return (end);
}

int	is_whitespace(char c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}

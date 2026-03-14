/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antigravity <antigravity@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by antigravity       #+#    #+#             */
/*   Updated: 2025/12/26 12:00:00 by antigravity      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"





/*
 * Handle words (including quoted strings)
 */
static int	get_word_end(char *line, int i)
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

static int	handle_red_in(char *line, int i, t_token **head)
{
	if (line[i + 1] == '<')
	{
		append_token(head, new_token(ft_strdup("<<"), TOKEN_HEREDOC));
		return (i + 2);
	}
	append_token(head, new_token(ft_strdup("<"), TOKEN_REDIRECT_IN));
	return (i + 1);
}

static int	handle_red_out(char *line, int i, t_token **head)
{
	if (line[i + 1] == '>')
	{
		append_token(head, new_token(ft_strdup(">>"), TOKEN_APPEND));
		return (i + 2);
	}
	append_token(head, new_token(ft_strdup(">"), TOKEN_REDIRECT_OUT));
	return (i + 1);
}

int	handle_symbol(char *line, int i, t_token **head)
{
	if (line[i] == '|')
	{
		append_token(head, new_token(ft_strdup("|"), TOKEN_PIPE));
		return (i + 1);
	}
	if (line[i] == '<')
		return (handle_red_in(line, i, head));
	if (line[i] == '>')
		return (handle_red_out(line, i, head));
	return (i + 1);
}

/*
 * Main tokenizer function
 */
t_token	*tokenize(char *line, int last_status)
{
	t_token	*head;
	int		i;

	head = NULL;
	i = 0;
	while (line[i])
	{
		if (is_whitespace(line[i]))
			i++;
		else if (line[i] == '|' || line[i] == '<' || line[i] == '>')
			i = handle_symbol(line, i, &head);
		else
			i = handle_word(line, i, &head, last_status);
	}
	return (head);
}

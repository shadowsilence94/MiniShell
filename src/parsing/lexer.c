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

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

/*
 * Create a new token and append to list
 */
t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

void	append_token(t_token **head, t_token *new_t)
{
	t_token	*tmp;

	if (!*head)
	{
		*head = new_t;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_t;
}

/*
 * Handle words (including quoted strings)
 */
int	handle_word(char *line, int i, t_token **head)
{
	int		start;
	char	quote;
	char	*val;

	start = i;
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
	val = ft_substr(line, start, i - start);
	append_token(head, new_token(val, TOKEN_WORD));
	return (i);
}

/*
 * Handle operators: |, <, >, <<, >>
 */
int	handle_symbol(char *line, int i, t_token **head)
{
	if (line[i] == '|')
	{
		append_token(head, new_token(ft_strdup("|"), TOKEN_PIPE));
		return (i + 1);
	}
	if (line[i] == '<')
	{
		if (line[i + 1] == '<')
		{
			append_token(head, new_token(ft_strdup("<<"), TOKEN_HEREDOC));
			return (i + 2);
		}
		append_token(head, new_token(ft_strdup("<"), TOKEN_REDIRECT_IN));
		return (i + 1);
	}
	if (line[i] == '>')
	{
		if (line[i + 1] == '>')
		{
			append_token(head, new_token(ft_strdup(">>"), TOKEN_APPEND));
			return (i + 2);
		}
		append_token(head, new_token(ft_strdup(">"), TOKEN_REDIRECT_OUT));
		return (i + 1);
	}
	return (i + 1);
}

/*
 * Main tokenizer function
 */
t_token	*tokenize(char *line)
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
			i = handle_word(line, i, &head);
	}
	return (head);
}

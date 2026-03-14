/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antigrav <antigrav@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by antigrav          #+#    #+#             */
/*   Updated: 2025/12/26 12:00:00 by antigrav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

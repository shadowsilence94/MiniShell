/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/14 21:10:00 by hko-ko           ###   ########.fr       */
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

static int	handle_logical_or_paren(char *line, int i, t_token **head)
{
	if (line[i] == '|' && line[i + 1] == '|')
	{
		append_token(head, new_token(ft_strdup("||"), TOKEN_OR));
		return (i + 2);
	}
	if (line[i] == '&' && line[i + 1] == '&')
	{
		append_token(head, new_token(ft_strdup("&&"), TOKEN_AND));
		return (i + 2);
	}
	if (line[i] == '(')
	{
		append_token(head, new_token(ft_strdup("("), TOKEN_L_PAREN));
		return (i + 1);
	}
	if (line[i] == ')')
	{
		append_token(head, new_token(ft_strdup(")"), TOKEN_R_PAREN));
		return (i + 1);
	}
	return (0);
}

int	handle_symbol(char *line, int i, t_token **head)
{
	int	res;

	res = handle_logical_or_paren(line, i, head);
	if (res != 0)
		return (res);
	if (line[i] == '|')
	{
		append_token(head, new_token(ft_strdup("|"), TOKEN_PIPE));
		return (i + 1);
	}
	if (line[i] == ';')
	{
		append_token(head, new_token(ft_strdup(";"), TOKEN_SEMI));
		return (i + 1);
	}
	if (line[i] == '<')
		return (handle_red_in(line, i, head));
	if (line[i] == '>')
		return (handle_red_out(line, i, head));
	return (i + 1);
}

t_token	*tokenize(char *line, t_exec_params *params)
{
	t_token	*head;
	int		i;

	head = NULL;
	i = 0;
	while (line[i])
	{
		if (is_whitespace(line[i]))
			i++;
		else if (ft_strchr("|&<>();", line[i]))
			i = handle_symbol(line, i, &head);
		else
		{
			i = handle_word(line, i, &head, params);
			if (i == -1)
			{
				ft_putendl_fd("minishell: syntax error: unclosed quote", 2);
				*params->last_status = 2;
				free_tokens(head);
				return (NULL);
			}
		}
	}
	return (head);
}

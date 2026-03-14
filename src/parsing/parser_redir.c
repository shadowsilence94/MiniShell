/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 22:35:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/14 22:35:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_redir_syntax(t_token *tmp)
{
	if (tmp->type >= TOKEN_REDIRECT_IN && tmp->type <= TOKEN_HEREDOC)
	{
		if (!tmp->next || tmp->next->type != TOKEN_WORD)
		{
			if (tmp->next)
				return (print_err(tmp->next->value));
			return (print_err("newline"));
		}
	}
	return (0);
}

static int	is_logic_op(t_token_type type)
{
	return (type == TOKEN_PIPE || type == TOKEN_AND || type == TOKEN_OR);
}

static int	check_parens_and_logic(t_token *tmp, int *parens)
{
	if (tmp->type == TOKEN_L_PAREN)
	{
		(*parens)++;
		if (tmp->next && tmp->next->type == TOKEN_R_PAREN)
			return (print_err(")"));
	}
	else if (tmp->type == TOKEN_R_PAREN)
		(*parens)--;
	if (*parens < 0)
		return (print_err(")"));
	if (is_logic_op(tmp->type) && (!tmp->next || is_logic_op(tmp->next->type)))
	{
		if (!tmp->next)
			return (print_err("newline"));
		return (print_err(tmp->next->value));
	}
	return (0);
}

int	validate_syntax(t_token *tokens)
{
	t_token	*tmp;
	int		parens;

	parens = 0;
	if (!tokens)
		return (0);
	if (is_logic_op(tokens->type) || tokens->type == TOKEN_R_PAREN)
		return (print_err(tokens->value));
	tmp = tokens;
	while (tmp)
	{
		if (check_parens_and_logic(tmp, &parens))
			return (1);
		if (check_redir_syntax(tmp))
			return (1);
		tmp = tmp->next;
	}
	if (parens > 0)
		return (print_err("newline"));
	return (0);
}

void	add_redirection(t_command *cmd, t_token *token, t_token *file_token)
{
	t_redir	*redir;
	t_redir	*last;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return ;
	redir->type = (t_redir_type)(token->type - TOKEN_REDIRECT_IN);
	redir->filename = ft_strdup(file_token->value);
	redir->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = redir;
	else
	{
		last = cmd->redirs;
		while (last->next)
			last = last->next;
		last->next = redir;
	}
}

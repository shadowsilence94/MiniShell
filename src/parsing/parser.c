/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/14 21:10:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir_type	get_redir_type(t_token_type type)
{
	if (type == TOKEN_REDIRECT_IN)
		return (REDIR_IN);
	if (type == TOKEN_REDIRECT_OUT)
		return (REDIR_OUT);
	if (type == TOKEN_APPEND)
		return (REDIR_APPEND);
	return (REDIR_HEREDOC);
}

void	add_redirection(t_command *cmd, t_token *token, t_token *file_token)
{
	t_redir	*redir;
	t_redir	*last;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return ;
	redir->type = get_redir_type(token->type);
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

static t_token	*handle_token(t_token *tmp, t_command **curr)
{
	if (tmp->type == TOKEN_PIPE)
	{
		(*curr)->next = new_command();
		(*curr)->next->prev = *curr;
		*curr = (*curr)->next;
	}
	else if (tmp->type == TOKEN_WORD)
		add_argument(*curr, tmp->value);
	else if (tmp->type >= TOKEN_REDIRECT_IN && tmp->type <= TOKEN_APPEND)
	{
		if (tmp->next && tmp->next->type == TOKEN_WORD)
		{
			add_redirection(*curr, tmp, tmp->next);
			tmp = tmp->next;
		}
		else
		{
			ft_putendl_fd("minishell: syntax error", 2);
			return (NULL);
		}
	}
	return (tmp->next);
}

t_command	*parse_input(char *line, char **envp, int *last_status)
{
	t_token			*tokens;
	t_token			*tmp;
	t_command		*head;
	t_command		*curr;
	t_exec_params	params;

	params.envp = &envp;
	params.last_status = last_status;
	tokens = tokenize(line, &params);
	if (!tokens)
		return (NULL);
	head = new_command();
	curr = head;
	tmp = tokens;
	while (tmp)
	{
		tmp = handle_token(tmp, &curr);
		if (!tmp && tokens)
			break ;
	}
	return (head);
}

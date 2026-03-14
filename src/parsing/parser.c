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

static void	add_infile(t_command *cmd, t_token *token, t_token *file_token)
{
	t_infile	*in;
	t_infile	*last_in;

	in = (t_infile *)malloc(sizeof(t_infile));
	in->filename = ft_strdup(file_token->value);
	in->is_heredoc = (token->type == TOKEN_HEREDOC);
	in->next = NULL;
	if (!cmd->infiles)
		cmd->infiles = in;
	else
	{
		last_in = cmd->infiles;
		while (last_in->next)
			last_in = last_in->next;
		last_in->next = in;
	}
}

static void	add_outfile(t_command *cmd, t_token *token, t_token *file_token)
{
	t_outfile	*out;
	t_outfile	*last_out;

	out = (t_outfile *)malloc(sizeof(t_outfile));
	out->filename = ft_strdup(file_token->value);
	out->is_append = (token->type == TOKEN_APPEND);
	out->next = NULL;
	if (!cmd->outfiles)
		cmd->outfiles = out;
	else
	{
		last_out = cmd->outfiles;
		while (last_out->next)
			last_out = last_out->next;
		last_out->next = out;
	}
}

void	add_redirection(t_command *cmd, t_token *token, t_token *file_token)
{
	if (token->type == TOKEN_REDIRECT_IN || token->type == TOKEN_HEREDOC)
		add_infile(cmd, token, file_token);
	else
		add_outfile(cmd, token, file_token);
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

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

static t_token	*handle_token(t_token *tmp, t_command **curr)
{
	if (tmp->type == TOKEN_PIPE || tmp->type == TOKEN_AND || tmp->type == TOKEN_OR)
	{
		if (tmp->type == TOKEN_AND)
			(*curr)->logic = LOGIC_AND;
		else if (tmp->type == TOKEN_OR)
			(*curr)->logic = LOGIC_OR;
		(*curr)->next = new_command();
		(*curr)->next->prev = *curr;
		*curr = (*curr)->next;
	}
	else if (tmp->type == TOKEN_WORD)
		add_argument(*curr, tmp->value);
	else if (tmp->type >= TOKEN_REDIRECT_IN && tmp->type <= TOKEN_HEREDOC)
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

static t_token	*init_parse(char *line, char **envp, int *last_status)
{
	t_token			*tokens;
	t_exec_params	params;

	params.envp = &envp;
	params.last_status = last_status;
	tokens = tokenize(line, &params);
	if (!tokens)
		return (NULL);
	if (validate_syntax(tokens))
	{
		*last_status = 2;
		return (NULL);
	}
	return (tokens);
}

static t_token	*extract_subtokens(t_token *start, t_token *end)
{
	t_token	*head;
	t_token	*curr;

	head = NULL;
	curr = start;
	while (curr && curr != end)
	{
		append_token(&head, new_token(ft_strdup(curr->value), curr->type));
		curr = curr->next;
	}
	return (head);
}

t_command	*parse_tokens(t_token *tokens)
{
	t_token			*tmp;
	t_command		*head;
	t_command		*curr;
	int				paren_count;

	if (!tokens)
		return (NULL);
	head = new_command();
	curr = head;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_L_PAREN)
		{
			t_token *sub_start = tmp->next;
			paren_count = 1;
			while (tmp->next && paren_count > 0)
			{
				tmp = tmp->next;
				if (tmp->type == TOKEN_L_PAREN) paren_count++;
				if (tmp->type == TOKEN_R_PAREN) paren_count--;
			}
			t_token *sub_tokens = extract_subtokens(sub_start, tmp);
			curr->sub_cmd = parse_tokens(sub_tokens);
			free_tokens(sub_tokens);
			tmp = tmp->next;
			if (tmp && (tmp->type == TOKEN_PIPE || tmp->type == TOKEN_AND || tmp->type == TOKEN_OR))
				continue;
			if (!tmp) break;
		}
		tmp = handle_token(tmp, &curr);
		if (!tmp)
			break ;
	}
	return (head);
}

t_command	*parse_input(char *line, char **envp, int *last_status)
{
	t_token			*tokens;
	t_command		*cmd_head;

	tokens = init_parse(line, envp, last_status);
	if (!tokens)
		return (NULL);
	cmd_head = parse_tokens(tokens);
	free_tokens(tokens);
	return (cmd_head);
}

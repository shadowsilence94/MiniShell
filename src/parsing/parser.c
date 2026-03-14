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

static t_token	*handle_token(t_token *t, t_command **c)
{
	if (t->type == TOKEN_PIPE || t->type == TOKEN_AND || t->type == TOKEN_OR)
	{
		if (t->type == TOKEN_AND)
			(*c)->logic = LOGIC_AND;
		else if (t->type == TOKEN_OR)
			(*c)->logic = LOGIC_OR;
		(*c)->next = new_command();
		(*c)->next->prev = *c;
		*c = (*c)->next;
	}
	else if (t->type == TOKEN_WORD)
		add_argument(*c, t->value);
	else if (t->type >= TOKEN_REDIRECT_IN && t->type <= TOKEN_HEREDOC)
	{
		if (!t->next || t->next->type != TOKEN_WORD)
		{
			ft_putendl_fd("minishell: syntax error", 2);
			return (NULL);
		}
		add_redirection(*c, t, t->next);
		return (t->next->next);
	}
	return (t->next);
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

static t_token	*handle_subshell(t_token *tmp, t_command **curr)
{
	int		paren_count;
	t_token	*sub_start;
	t_token	*sub_tokens;

	sub_start = tmp->next;
	paren_count = 1;
	while (tmp->next && paren_count > 0)
	{
		tmp = tmp->next;
		if (tmp->type == TOKEN_L_PAREN)
			paren_count++;
		if (tmp->type == TOKEN_R_PAREN)
			paren_count--;
	}
	sub_tokens = extract_subtokens(sub_start, tmp);
	(*curr)->sub_cmd = parse_tokens(sub_tokens);
	free_tokens(sub_tokens);
	return (tmp->next);
}

t_command	*parse_tokens(t_token *tokens)
{
	t_token			*tmp;
	t_command		*head;
	t_command		*curr;

	if (!tokens)
		return (NULL);
	head = new_command();
	curr = head;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_L_PAREN)
		{
			tmp = handle_subshell(tmp, &curr);
			if (tmp && (tmp->type == TOKEN_PIPE || tmp->type == TOKEN_AND
					|| tmp->type == TOKEN_OR))
				continue ;
			if (!tmp)
				break ;
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

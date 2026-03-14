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
	if (tmp->type == TOKEN_PIPE)
	{
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

t_command	*parse_input(char *line, char **envp, int *last_status)
{
	t_token			*tokens;
	t_token			*tmp;
	t_command		*head;
	t_command		*curr;

	tokens = init_parse(line, envp, last_status);
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

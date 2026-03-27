/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/15 12:00:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	should_skip(t_logical_op op, int status)
{
	if (op == LOGIC_AND && status != 0)
		return (1);
	if (op == LOGIC_OR && status == 0)
		return (1);
	return (0);
}

void	execute_commands(t_command *cmd, char ***envp, int *last_status)
{
	t_command		*curr;
	t_command		*next_p;
	t_command		*stored;
	t_logical_op	op;

	curr = cmd;
	while (curr)
	{
		next_p = curr;
		while (next_p && next_p->logic == LOGIC_NONE && next_p->next)
			next_p = next_p->next;
		stored = next_p->next;
		op = next_p->logic;
		next_p->next = NULL;
		execute_command_node(curr, envp, last_status);
		next_p->next = stored;
		if (op != LOGIC_SEQ && should_skip(op, *last_status))
			break ;
		curr = stored;
	}
}

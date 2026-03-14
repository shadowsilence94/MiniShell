/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/14 21:10:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_assignment(t_command *cmd, char ***envp, int *last_status)
{
	char	*equal;
	char	*key;
	char	*val;

	if (!cmd->args || !cmd->args[0])
		return (0);
	equal = ft_strchr(cmd->args[0], '=');
	if (equal && equal > cmd->args[0] && !cmd->args[1])
	{
		key = ft_substr(cmd->args[0], 0, equal - cmd->args[0]);
		val = ft_strdup(equal + 1);
		set_env(envp, key, val);
		free(key);
		free(val);
		return (*last_status = 0, 1);
	}
	if (cmd->args[0] && cmd->args[1] && ft_strncmp(cmd->args[1], "=", 2) == 0
		&& cmd->args[2])
	{
		set_env(envp, cmd->args[0], cmd->args[2]);
		return (*last_status = 0, 1);
	}
	return (0);
}

static void	handle_single_cmd(t_command *cmd, char ***envp, int *last_status)
{
	int	saved_stdout;
	int	saved_stdin;

	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	*last_status = handle_redirections(cmd);
	if (*last_status == 0)
		run_single_builtin(cmd, envp, last_status);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
}

static void	execute_command_node(t_command *curr, char ***envp,
				int *last_status)
{
	if (!curr->args && curr->redirs)
		handle_single_cmd(curr, envp, last_status);
	else if (curr->args && curr->args[0])
	{
		if (handle_assignment(curr, envp, last_status))
			(void)0;
		else if (!curr->next && is_builtin(curr->args[0]))
			handle_single_cmd(curr, envp, last_status);
		else
			handle_process_loop(curr, envp, last_status);
	}
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
		if ((op == LOGIC_AND && *last_status != 0)
			|| (op == LOGIC_OR && *last_status == 0))
			break ;
		curr = stored;
	}
}

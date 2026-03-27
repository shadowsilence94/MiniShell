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

static int	do_assign(t_command *cmd, char ***envp, int *last_status)
{
	char	*equal;
	char	*key;
	char	*val;

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
	return (0);
}

static int	handle_assignment(t_command *cmd, char ***envp, int *last_status)
{
	if (!cmd->args || !cmd->args[0])
		return (0);
	if (do_assign(cmd, envp, last_status))
		return (1);
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
	int				s_out;
	int				s_in;
	t_exec_params	params;

	params.envp = envp;
	params.last_status = last_status;
	params.prev_fd = -1;
	params.p_fd[0] = -1;
	params.p_fd[1] = -1;
	params.sq = false;
	params.dq = false;
	params.any_q = false;
	s_out = dup(STDOUT_FILENO);
	s_in = dup(STDIN_FILENO);
	*last_status = handle_redirections(cmd, &params);
	if (*last_status == 0)
		run_single_builtin(cmd, envp, last_status);
	dup2(s_out, STDOUT_FILENO);
	dup2(s_in, STDIN_FILENO);
	close(s_out);
	close(s_in);
}

void	execute_command_node(t_command *curr, char ***envp,
				int *last_status)
{
	expand_cmd_wildcards(curr);
	if (curr->sub_cmd && !curr->next)
		handle_process_loop(curr, envp, last_status);
	else if (!curr->args && curr->redirs && !curr->next)
		handle_single_cmd(curr, envp, last_status);
	else if (curr->args && curr->args[0] && !curr->next
		&& is_builtin(curr->args[0]))
	{
		if (!handle_assignment(curr, envp, last_status))
			handle_single_cmd(curr, envp, last_status);
	}
	else if ((curr->args && curr->args[0]) || curr->redirs || curr->next
		|| curr->sub_cmd)
	{
		if (curr->args && curr->args[0]
			&& handle_assignment(curr, envp, last_status))
			(void)0;
		else
			handle_process_loop(curr, envp, last_status);
	}
}

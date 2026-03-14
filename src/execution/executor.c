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

static t_command	*setup_pipe(t_command *cmd, int pipe_fd[2])
{
	if (cmd->next && pipe(pipe_fd) == -1)
		return (NULL);
	else if (!cmd->next)
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
	}
	return (cmd);
}

static void	cleanup_parent(t_exec_params *p, t_command **cmd)
{
	if (p->prev_fd != -1)
		close(p->prev_fd);
	if (p->p_fd[1] != -1)
		close(p->p_fd[1]);
	p->prev_fd = p->p_fd[0];
	*cmd = (*cmd)->next;
}

static void	handle_process_loop(t_command *cmd, char ***envp, int *last_status)
{
	t_exec_params	p;
	pid_t			pid;

	p.envp = envp;
	p.last_status = last_status;
	p.prev_fd = -1;
	while (cmd)
	{
		if (!setup_pipe(cmd, p.p_fd))
			return ;
		pid = fork();
		if (pid == -1)
			return ;
		if (pid == 0)
			child_process(cmd, &p);
		else
			cleanup_parent(&p, &cmd);
	}
	wait_for_children(p.prev_fd, last_status);
}

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

void	execute_commands(t_command *cmd, char ***envp, int *last_status)
{
	if (!cmd->next)
	{
		if (!cmd->args && cmd->redirs)
		{
			int saved_stdout = dup(STDOUT_FILENO);
			int saved_stdin = dup(STDIN_FILENO);
			*last_status = handle_redirections(cmd);
			dup2(saved_stdout, STDOUT_FILENO);
			dup2(saved_stdin, STDIN_FILENO);
			close(saved_stdout);
			close(saved_stdin);
			return ;
		}
		if (cmd->args && cmd->args[0] && cmd->args[0][0] == '\0' && !cmd->args[1])
		{
			if (handle_assignment(cmd, envp, last_status))
				return ;
		}
		if (cmd->args)
		{
			if (handle_assignment(cmd, envp, last_status))
				return ;
			if (is_builtin(cmd->args[0]))
			{
				run_single_builtin(cmd, envp, last_status);
				return ;
			}
		}
		if (!cmd->args && !cmd->redirs)
			return ;
	}
	handle_process_loop(cmd, envp, last_status);
}

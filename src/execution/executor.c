/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2025/12/26 12:00:00 by hko-ko           ###   ########.fr       */
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

static void	handle_process_loop(t_command *cmd, char ***envp, int *last_status)
{
	int		p_fd[2];
	int		prev_p_fd;
	pid_t	pid;

	prev_p_fd = -1;
	while (cmd)
	{
		if (!setup_pipe(cmd, p_fd))
			return ;
		pid = fork();
		if (pid == -1)
			return ;
		if (pid == 0)
			child_process(cmd, envp, prev_p_fd, p_fd);
		else
		{
			if (prev_p_fd != -1)
				close(prev_p_fd);
			if (p_fd[1] != -1)
				close(p_fd[1]);
			prev_p_fd = p_fd[0];
			cmd = cmd->next;
		}
	}
	wait_for_children(prev_p_fd, last_status);
}

void	execute_commands(t_command *cmd, char ***envp, int *last_status)
{
	if (!cmd->next && cmd->args && cmd->args[0]
		&& (ft_strncmp(cmd->args[0], "cd", 3) == 0
			|| ft_strncmp(cmd->args[0], "exit", 5) == 0))
	{
		run_single_builtin(cmd, envp, last_status);
		return ;
	}
	handle_process_loop(cmd, envp, last_status);
}

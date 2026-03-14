/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 22:35:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/14 22:35:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*setup_pipe(t_command *cmd, int pipe_fd[2])
{
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (cmd->next && pipe(pipe_fd) == -1)
		return (NULL);
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

void	handle_process_loop(t_command *cmd, char ***envp, int *last_status)
{
	t_exec_params	p;
	pid_t			pid;
	pid_t			last_pid;

	p.envp = envp;
	p.last_status = last_status;
	p.prev_fd = -1;
	last_pid = -1;
	while (cmd)
	{
		if (!setup_pipe(cmd, p.p_fd))
			return ;
		pid = fork();
		if (pid == -1)
			return ;
		if (pid == 0)
			child_process(cmd, &p);
		last_pid = pid;
		cleanup_parent(&p, &cmd);
	}
	if (p.prev_fd != -1)
		close(p.prev_fd);
	wait_for_children(last_pid, last_status);
}

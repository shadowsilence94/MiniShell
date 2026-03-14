/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antigrav <antigrav@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 17:00:00 by antigrav       #+#    #+#             */
/*   Updated: 2026/03/14 17:00:00 by antigrav      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	wait_for_children(int prev_pipe_fd, int *last_status)
{
	int	status;
	if (prev_pipe_fd != -1)
		close(prev_pipe_fd);
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			*last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*last_status = 128 + WTERMSIG(status);
	}

}

void	run_command(t_command *cmd, char ***envp, int *last_status)
{
	char	*path;
	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, envp, last_status));
	path = find_command_path(cmd->args[0], *envp);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmd->args[0], 2);
		exit(127);
	}

	execve(path, cmd->args, *envp);
	perror("minishell");
	exit(1);
}

void	child_process(t_command *cmd, char ***envp, int prev_fd, int pipe_fd[2])
{
	int	l_status;
	l_status = 0;
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}

	if (pipe_fd[1] != -1)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		close(pipe_fd[0]);
	}

	if (handle_redirections(cmd))
		exit(1);
	run_command(cmd, envp, &l_status);
}


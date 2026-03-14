/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/14 21:10:00 by hko-ko           ###   ########.fr       */
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

static void	run_command(t_command *cmd, char ***envp)
{
	char	*path;

	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (cmd->args[0][0] == '\0')
	{
		ft_putendl_fd("minishell: : command not found", 2);
		exit(127);
	}
	if (ft_strchr(cmd->args[0], '/') && is_dir(cmd->args[0]))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putendl_fd(": is a directory", 2);
		exit(126);
	}
	path = find_command_path(cmd->args[0], *envp);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}
	execve(path, cmd->args, *envp);
	perror("execve");
	exit(1);
}

void	child_process(t_command *cmd, t_exec_params *params)
{
	if (params->prev_fd != -1)
	{
		dup2(params->prev_fd, STDIN_FILENO);
		close(params->prev_fd);
	}
	if (params->p_fd[1] != -1)
	{
		dup2(params->p_fd[1], STDOUT_FILENO);
		close(params->p_fd[1]);
	}
	if (params->p_fd[0] != -1)
		close(params->p_fd[0]);
	if (handle_redirections(cmd))
		exit(1);
	if (cmd->args && is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, params->envp, params->last_status));
	if (cmd->args)
		run_command(cmd, params->envp);
	exit(0);
}

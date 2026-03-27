/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/14 22:25:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_for_children(pid_t last_pid, int *last_status)
{
	int		status;
	pid_t	pid;

	pid = wait(&status);
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				*last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				*last_status = 128 + WTERMSIG(status);
		}
		pid = wait(&status);
	}
}

static void	cmd_not_found(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": command not found", 2);
	exit(127);
}

static char	*handle_abs_rel_path(t_command *cmd)
{
	if (access(cmd->args[0], F_OK) == -1)
	{
		perror(cmd->args[0]);
		exit(127);
	}
	if (is_dir(cmd->args[0]))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putendl_fd(": Is a directory", 2);
		exit(126);
	}
	if (access(cmd->args[0], X_OK) == -1)
	{
		perror(cmd->args[0]);
		exit(126);
	}
	return (ft_strdup(cmd->args[0]));
}

static void	run_command(t_command *cmd, char ***envp)
{
	char	*path;

	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (cmd->args[0][0] == '\0')
		cmd_not_found("");
	if (ft_strchr(cmd->args[0], '/'))
		path = handle_abs_rel_path(cmd);
	else
	{
		path = find_command_path(cmd->args[0], *envp);
		if (!path)
			cmd_not_found(cmd->args[0]);
	}
	execve(path, cmd->args, *envp);
	if (errno == ENOEXEC)
		exec_script(path, cmd->args, *envp);
	perror(path);
	exit(126);
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
	if (handle_redirections(cmd, params))
		exit(1);
	if (cmd->sub_cmd)
	{
		execute_commands(cmd->sub_cmd, params->envp, params->last_status);
		exit(*(params->last_status));
	}
	if (cmd->args && is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, params->envp, params->last_status));
	if (cmd->args)
		run_command(cmd, params->envp);
	exit(0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antigrav <antigrav@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 16:35:00 by antigrav       #+#    #+#             */
/*   Updated: 2026/03/14 16:35:00 by antigrav      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

int	execute_builtin(t_command *cmd, char ***envp, int *last_status)
{
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		return (ft_echo(cmd->args));
	if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		return (ft_cd(cmd->args, envp));
	if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		return (ft_pwd());
	if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		return (ft_env(*envp));
	if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		return (ft_exit(cmd->args, last_status));
	return (0);
}

int	run_single_builtin(t_command *cmd, char ***envp, int *last_status)
{
	int	original_stdin;
	int	original_stdout;
	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (handle_redirections(cmd) == 0)
		*last_status = execute_builtin(cmd, envp, last_status);
	else
		*last_status = 1;
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
	return (1);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antigravity <antigravity@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by antigravity       #+#    #+#             */
/*   Updated: 2025/12/26 12:00:00 by antigravity      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirections(t_command *cmd)
{
	t_infile	*in;
	t_outfile	*out;
	int			fd;

	in = cmd->infiles;
	while (in)
	{
		if (in->is_heredoc)
		{
		}
		else
		{
			fd = open(in->filename, O_RDONLY);
			if (fd == -1)
			{
				perror(in->filename);
				return (1);
			}
			if (dup2(fd, STDIN_FILENO) == -1)
				return (1);
			close(fd);
		}
		in = in->next;
	}
	out = cmd->outfiles;
	while (out)
	{
		if (out->is_append)
			fd = open(out->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(out->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror(out->filename);
			return (1);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (1);
		close(fd);
		out = out->next;
	}
	return (0);
}

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

void	child_process(t_command *cmd, char ***envp, int prev_pipe_fd, int pipe_fd[2], int *last_status)
{
	char	*path;

	if (prev_pipe_fd != -1)
	{
		dup2(prev_pipe_fd, STDIN_FILENO);
		close(prev_pipe_fd);
	}
	if (pipe_fd[1] != -1)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		close(pipe_fd[0]);
	}
	if (handle_redirections(cmd))
		exit(1);

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

static int	run_single_builtin(t_command *cmd, char ***envp, int *last_status)
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

static void	wait_for_children(int prev_pipe_fd, int *last_status)
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

static void	handle_process_loop(t_command *cmd, char ***envp, int *last_status)
{
	int		pipe_fd[2];
	int		prev_pipe_fd;
	pid_t	pid;

	prev_pipe_fd = -1;
	while (cmd)
	{
		if (cmd->next && pipe(pipe_fd) == -1)
			return ;
		else if (!cmd->next)
		{
			pipe_fd[0] = -1;
			pipe_fd[1] = -1;
		}
		pid = fork();
		if (pid == -1)
			return ;
		if (pid == 0)
			child_process(cmd, envp, prev_pipe_fd, pipe_fd, last_status);
		else
		{
			if (prev_pipe_fd != -1)
				close(prev_pipe_fd);
			if (pipe_fd[1] != -1)
				close(pipe_fd[1]);
			prev_pipe_fd = pipe_fd[0];
			cmd = cmd->next;
		}
	}
	wait_for_children(prev_pipe_fd, last_status);
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


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/14 22:25:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received;

static void	process_input(char *line, char ***envp, int *last_status);
static void	init_shell(char ***env_vars, char **envp);
static void	shell_loop(char ***env_vars, int *last_status);

static void	init_shell(char ***env_vars, char **envp)
{
	*env_vars = copy_env(envp);
	increment_shlvl(env_vars);
	setup_signals();
}

static void	shell_loop(char ***env_vars, int *last_status)
{
	char	*line;

	while (1)
	{
		g_signal_received = 0;
		line = readline(PROMPT);
		if (!line)
		{
			if (g_signal_received)
			{
				*last_status = 130;
				continue ;
			}
			if (isatty(STDIN_FILENO))
				ft_putendl_fd("exit", 1);
			break ;
		}
		if (g_signal_received)
			*last_status = 130;
		process_input(line, env_vars, last_status);
		free(line);
	}
}

static void	process_input(char *line, char ***envp, int *last_status)
{
	t_command	*cmd_list;

	if (line && *line)
		add_history(line);
	cmd_list = parse_input(line, *envp, last_status);
	if (cmd_list)
	{
		execute_commands(cmd_list, envp, last_status);
		free_command_list(cmd_list);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	**env_vars;
	int		last_status;

	(void)argc;
	(void)argv;
	init_shell(&env_vars, envp);
	last_status = 0;
	g_signal_received = 0;
	shell_loop(&env_vars, &last_status);
	rl_clear_history();
	free_split(env_vars);
	return (last_status);
}

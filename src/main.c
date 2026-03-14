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

volatile sig_atomic_t	g_signal_received = 0;

static void	init_shell(char ***env_vars, char **envp)
{
	*env_vars = copy_env(envp);
	increment_shlvl(env_vars);
	setup_signals();
	rl_bind_key('\t', rl_complete);
}

void	process_input(char *line, char ***envp, int *last_status)
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
	char	*line;
	char	**env_vars;
	int		last_status;

	(void)argc;
	(void)argv;
	init_shell(&env_vars, envp);
	last_status = 0;
	while (1)
	{
		line = readline(PROMPT);
		if (!line)
		{
			if (isatty(STDIN_FILENO))
				printf("exit\n");
			break ;
		}
		process_input(line, &env_vars, &last_status);
		free(line);
	}
	rl_clear_history();
	free_split(env_vars);
	return (last_status);
}

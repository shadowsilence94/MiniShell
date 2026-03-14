/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2025/12/26 12:00:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

static void	print_redirections(t_command *cmd)
{
	t_infile	*in;
	t_outfile	*out;

	in = cmd->infiles;
	while (in)
	{
		printf("  Infile: %s (Heredoc: %d)\n", in->filename, in->is_heredoc);
		in = in->next;
	}
	out = cmd->outfiles;
	while (out)
	{
		printf("  Outfile: %s (Append: %d)\n", out->filename, out->is_append);
		out = out->next;
	}
}

void	print_commands(t_command *cmd)
{
	int	i;

	while (cmd)
	{
		printf("Command:\n");
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				printf("  Arg[%d]: %s\n", i, cmd->args[i]);
				i++;
			}
		}
		print_redirections(cmd);
		if (cmd->next)
			printf("  | PIPE |\n");
		cmd = cmd->next;
	}
}

/*
 * Process input line
 */
void	process_input(char *line, char ***envp, int *last_status)
{
	t_command	*cmd_list;

	if (line && *line)
		add_history(line);
	cmd_list = parse_input(line, *envp, last_status);
	if (cmd_list)
	{
		execute_commands(cmd_list, envp, last_status);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**env_vars;
	int		last_status;

	(void)argc;
	(void)argv;
	env_vars = envp;
	last_status = 0;
	setup_signals();
	while (1)
	{
		line = readline(PROMPT);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		process_input(line, &env_vars, &last_status);
		free(line);
	}
	rl_clear_history();
	return (0);
}

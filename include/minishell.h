/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antigravity <antigravity@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by antigravity       #+#    #+#             */
/*   Updated: 2025/12/26 12:00:00 by antigravity      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

# define PROMPT "minishell$ "

/*
 * Global variable for signal handling
 */
extern volatile sig_atomic_t	g_signal_received;

/*
 * Enum for token types
 */
typedef enum s_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

/*
 * Struct for input file
 */
typedef struct s_infile
{
	char			*filename;
	bool			is_heredoc;
	char			*limiter;
	struct s_infile	*next;
}	t_infile;

/*
 * Struct for output file
 */
typedef struct s_outfile
{
	char				*filename;
	bool				is_append;
	struct s_outfile	*next;
}	t_outfile;

/*
 * Struct for command node
 */
typedef struct s_command
{
	char				**args;
	char				*cmd_path;
	t_infile			*infiles;
	t_outfile			*outfiles;
	int					fd_in;
	int					fd_out;
	struct s_command	*next;
	struct s_command	*prev;
}	t_command;


/*
 * Parsing & Lexing
 */
t_token		*tokenize(char *line, int last_status);
t_command	*parse_input(char *line, char **envp, int *last_status);

/*
 * Execution
 */
char		*find_command_path(char *cmd, char **envp);
void		execute_commands(t_command *cmd_list, char ***envp, int *last_status);

/*
 * Builtins
 */
int			ft_echo(char **args);
int			ft_pwd(void);
int			ft_cd(char **args, char ***envp);
int			ft_env(char **envp);
int			ft_exit(char **args, int *last_status);

/*
 * Cleanup
 */
void		free_command_list(t_command *cmd_list);

/*
 * Signals
 */
void		setup_signals(void);

#endif

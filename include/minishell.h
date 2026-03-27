/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/14 21:10:00 by hko-ko           ###   ########.fr       */
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

/* Readline/Libedit compatibility */
void			rl_replace_line(const char *text, int clear_undo);
void			rl_clear_history(void);

# define PROMPT "minishell$ "

/*
 * Global variable for signal handling
 */
extern volatile sig_atomic_t	g_signal_received;

/*
 * Enum for token types
 */
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_L_PAREN,
	TOKEN_R_PAREN,
	TOKEN_SEMI,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	char				*value;
	t_token_type		type;
	struct s_token		*next;
}	t_token;

/*
 * Struct for execution parameters
 */
typedef struct s_exec_params
{
	char	***envp;
	int		*last_status;
	int		prev_fd;
	int		p_fd[2];
	bool	sq;
	bool	dq;
	bool	any_q;
}	t_exec_params;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*filename;
	int				quoted;
	struct s_redir	*next;
}	t_redir;

typedef enum e_logical_op
{
	LOGIC_NONE,
	LOGIC_AND,
	LOGIC_OR,
	LOGIC_SEQ
}	t_logical_op;

/*
 * Struct for command node
 */
typedef struct s_command
{
	char				**args;
	char				*cmd_path;
	t_redir				*redirs;
	int					fd_in;
	int					fd_out;
	t_logical_op		logic;
	struct s_command	*sub_cmd;
	struct s_command	*next;
	struct s_command	*prev;
}	t_command;

/*
 * Parsing & Lexing
 */
t_token					*tokenize(char *line, t_exec_params *params);
t_command				*parse_input(char *line, char **envp, int *last_status);
t_command				*parse_tokens(t_token *tokens);
int						validate_syntax(t_token *tokens);
int						is_whitespace(char c);
t_token					*new_token(char *value, t_token_type type);
void					append_token(t_token **head, t_token *new_t);
void					free_tokens(t_token *tokens);
int						print_err(char *token);
char					*expand_status(char *val, t_exec_params *params);
t_command				*new_command(void);
void					add_argument(t_command *cmd, char *arg);
void					add_redirection(t_command *cmd, t_token *token,
							t_token *file_token);
int						get_word_end(char *line, int i);
int						handle_word(char *line, int i, t_token **head,
							t_exec_params *params);
t_token					*expand_wildcard(char *pattern);
int						has_unquoted_wildcard(char *str);
int						has_unquoted_var(char *str);
int						count_wildcard_matches(char *pattern);
void					expand_cmd_wildcards(t_command *cmd);
char					*append_char(char *res, char c);
t_token					*extract_subtokens(t_token *start, t_token *end);
void					sort_strings(char **arr, int count);
char					*prepare_pattern(char *pattern);
void					clean_marker(char *pattern);
int						get_expanded_count(char *arg);
void					fill_wildcard_list(char *arg, char **new_args, int *c);
void					toggle_quotes(char c, bool *s_quote, bool *d_quote);
char					*apply_expansion(char *res, char *var_val);
bool					is_expandable(char *v, int i, bool sq);
void					handle_status_quotes(char c, bool q[3], int *i);
char					*expand_heredoc_line(char *line, t_exec_params *params);
char					*handle_expansion(char *val, int *i, t_exec_params *p,
							bool dq);

/*
 * Execution
 */
char					*find_command_path(char *cmd, char **envp);
void					execute_commands(t_command *cmd_list, char ***envp,
							int *last_status);
void					handle_process_loop(t_command *cmd, char ***envp,
							int *last_status);
int						is_builtin(char *cmd);
int						execute_builtin(t_command *cmd, char ***envp,
							int *last_status);
void					run_single_builtin(t_command *cmd, char ***envp,
							int *last_status);
int						handle_redirections(t_command *cmd,
							t_exec_params *params);
void					wait_for_children(pid_t last_pid, int *last_status);
void					execute_command_node(t_command *curr, char ***envp,
							int *last_status);
void					child_process(t_command *cmd, t_exec_params *params);
int						handle_in(t_redir *redir);
int						handle_out(t_redir *redir);
int						handle_heredoc(t_redir *redir, t_exec_params *params);
int						is_dir(char *path);
void					exec_script(char *path, char **args, char **envp);

/*
 * Builtins
 */
int						ft_echo(char **args);
int						ft_pwd(void);
int						ft_cd(char **args, char ***envp);
int						ft_env(char **envp);
int						ft_export(char **args, char ***envp);
int						ft_unset(char **args, char ***envp);
int						ft_exit(char **args, int *last_status);
int						is_all_digits(char *str);
long long				ft_atoll(const char *str);

/*
 * Cleanup
 */
void					free_command_list(t_command *cmd_list);
void					free_split(char **split);
void					print_export(char **envp);

/*
 * Environment Utilities
 */
char					**copy_env(char **envp);
char					*get_env_value(char **envp, char *key);
void					set_env(char ***envp, char *key, char *value);
void					increment_shlvl(char ***envp);
char					*get_var_value(char *name, char **envp,
							int last_status);
void					remove_var(char ***envp, int j);

/*
 * Signals
 */
void					setup_signals(void);

#endif

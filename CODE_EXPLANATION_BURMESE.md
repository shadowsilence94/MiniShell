# Minishell အသေးစိတ် ကုဒ်ရှင်းလင်းချက် (Exhaustive Burmese Explanation)

ဤစာတမ်းတွင် Minishell ပရောဂျက်ရှိ ကုဒ်ဖိုင်အားလုံး၏ ကုဒ်များကို တစ်ကြောင်းချင်းစီ မြန်မာဘာသာဖြင့် အသေးစိတ် ရှင်းလင်းထားပါသည်။

---

## ၁။ `include/minishell.h` (Header File)

```c
#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>       // printf, perror အတွက်
# include <stdlib.h>      // malloc, free, exit အတွက်
# include <unistd.h>      // fork, execve, pipe, dup2 အတွက်
# include <stdbool.h>     // bool (true/false) အတွက်
# include <fcntl.h>       // open, O_RDONLY အတွက်
# include <sys/wait.h>    // wait, waitpid အတွက်
# include <sys/stat.h>    // stat အတွက်
# include <signal.h>      // signal, sigaction အတွက်
# include <errno.h>       // errno အတွက်
# include <readline/readline.h> // readline အတွက်
# include <readline/history.h>  // add_history အတွက်
# include "../libft/libft.h"    // libft functions များအတွက်

# define PROMPT "minishell$ " // Prompt စာသား

// Signal အတွက် global variable (စနစ်မှ signal ရလာလျှင် သိရှိစေရန်)
extern volatile sig_atomic_t	g_signal_received;

// Token အမျိုးအစားများကို Enum သတ်မှတ်ချက်
typedef enum s_token_type
{
	TOKEN_WORD,           // စာလုံး (echo, ls, etc)
	TOKEN_PIPE,           // |
	TOKEN_REDIRECT_IN,    // <
	TOKEN_REDIRECT_OUT,   // >
	TOKEN_APPEND,         // >>
	TOKEN_HEREDOC,       // <<
	TOKEN_AND,            // &&
	TOKEN_OR,             // ||
	TOKEN_L_PAREN,        // (
	TOKEN_R_PAREN,        // )
	TOKEN_SEMI,           // ; (Sequential operator)
	TOKEN_EOF             // End of File
}	t_token_type;

// Token structure (Link list ပုံစံ)
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

// Execution အတွက် လိုအပ်သော parameters များ
typedef struct s_exec_params
{
	char	***envp;       // Environment variables pointer
	int		*last_status;  // နောက်ဆုံး exit status
	int		prev_fd;       // Pipe အတွက် ယခင် pipe ရဲ့ read end
	int		p_fd[2];       // လက်ရှိ pipe ရဲ့ fds
}	t_exec_params;

// Redirection အမျိုးအစားများ
typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

// Redirection structure
typedef struct s_redir
{
	t_redir_type	type;
	char			*filename;
	struct s_redir	*next;
}	t_redir;

// Logical Operators (&& / ||)
typedef enum e_logical_op
{
	LOGIC_NONE,           // Pipe သို့မဟုတ် သာမန် command
	LOGIC_AND,            // &&
	LOGIC_OR,             // ||
	LOGIC_SEQ             // ;
}	t_logical_op;

// Command Structure (parsing ပြီးထွက်လာမည့် list)
typedef struct s_command
{
	char				**args;     // Command arguments (ls, -l, etc)
	char				*cmd_path;  // Binary ရဲ့ လမ်းကြောင်းအပြည့်
	t_redir				*redirs;    // Redirections list
	int					fd_in;      // Input FD
	int					fd_out;     // Output FD
	t_logical_op		logic;      // && သို့မဟုတ် ||
	struct s_command	*sub_cmd;   // Subshell အတွင်းမှ commands များ ( )
	struct s_command	*next;      // နောက်ထပ် command (Pipe ပါလျှင်)
	struct s_command	*prev;      // ယခင် command
}	t_command;

// ... (Function Prototypes များ)
#endif
```

---

## ၂။ `src/main.c` (EntryPoint)

```c
#include "minishell.h"

// Global signal variable initialize လုပ်ခြင်း
volatile sig_atomic_t	g_signal_received = 0;

// Shell စတင်ရန် လိုအပ်သည်များကို ပြင်ဆင်ခြင်း
static void	init_shell(char ***env_vars, char **envp)
{
	*env_vars = copy_env(envp);     // Environment ကို Clone ကူးယူသည်
	increment_shlvl(env_vars);      // SHLVL ကို ၁ တိုးသည်
	setup_signals();                // Ctrl+C စသည်တို့ကို handle လုပ်ရန် ပြင်သည်
	rl_bind_key('\t', rl_complete); // Tab key အတွက် autocomplete စနစ်
}

// User input ကို လက်ခံပြီး process လုပ်ခြင်း
void	process_input(char *line, char ***envp, int *last_status)
{
	t_command	*cmd_list;

	if (line && *line)               // input ရှိခဲ့လျှင်
		add_history(line);           // history ထဲထည့်သည်
	cmd_list = parse_input(line, *envp, last_status); // Parsing လုပ်သည်
	if (cmd_list)
	{
		execute_commands(cmd_list, envp, last_status); // Execution လုပ်သည်
		free_command_list(cmd_list);  // Memory ပြန်လွှတ်သည်
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**env_vars;
	int		last_status;

	(void)argc;                      // argc ကို ignore လုပ်သည်
	(void)argv;                      // argv ကို ignore လုပ်သည်
	init_shell(&env_vars, envp);    // Shell ပြင်ဆင်သည်
	last_status = 0;                 // စတင်ချိန် status က 0
	while (1)                        // Shell Loop စတင်သည်
	{
		line = readline(PROMPT);     // minishell$ ဆိုပြီး input တောင်းသည်
		if (!line)                   // Ctrl+D နှိပ်လျှင်
		{
			if (isatty(STDIN_FILENO))
				printf("exit\n");
			break ;                  // Loop မှထွက်သည်
		}
		process_input(line, &env_vars, &last_status); // input ကိုင်တွယ်သည်
		free(line);                  // Line memory လွှတ်သည်
	}
	rl_clear_history();              // History ရှင်းသည်
	free_split(env_vars);            // Environment ရှင်းသည်
	return (last_status);            // ပြီးဆုံးသည်
}
```

---

## ၃။ `src/parsing/expand_utils.c` (Expansion & Markers)

**နားလည်ရန်:** 
- `\1` Marker = အဖြေအလွတ်ထွက်သော unquoted expansion (ဥပမာ `$NONEXISTENT`)
- `\2` Marker = Unquoted wildcard `*` (Execution-time မှ ဖြေရှင်းရန်)

```c
// Variable နမည်ကို ခွဲထုတ်ခြင်း (ဥပမာ $USER -> USER)
static char	*get_var_name(char *val, int *i)
{
	int		start;
	char	*name;

	start = ++(*i);
	if (val[*i] == '?') // $? ဖြစ်လျှင်
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	while (val[*i] && (ft_isalnum(val[*i]) || val[*i] == '_'))
		(*i)++;
	name = ft_substr(val, start, *i - start);
	return (name);
}

// Variable ကို တကယ် expansion လုပ်ခြင်း
static char	*handle_expansion(char *res, char *val, int *i, t_exec_params *p, bool dq)
{
	char	*name;
	char	*var_val;
	char	*tmp;
	int		j;

	name = get_var_name(val, i);
	var_val = get_var_value(name, *p->envp, *p->last_status); // တန်ဖိုးရှာသည်
	if (!var_val)
		var_val = ft_strdup("");
	if (!dq) // Double quotes ထဲမှာမဟုတ်လျှင်
	{
		j = -1;
		while (var_val[++j])
			if (var_val[j] == '*')
				var_val[j] = '\2'; // * ပါလာလျှင် \2 marker ပြောင်းသည်
	}
	tmp = ft_strjoin(res, var_val); // ရှိပြီးသား text နဲ့ ပေါင်းသည်
	free(res);
	free(name);
	free(var_val);
	return (tmp);
}

// Quote များ အဖွင့်အပိတ် စစ်ဆေးခြင်း
static void	toggle_quotes(char c, bool *s_quote, bool *d_quote)
{
	if (c == '\'' && !*d_quote)
		*s_quote = !*s_quote;
	else if (c == '"' && !*s_quote)
		*d_quote = !*d_quote;
}

// $ ပါလျှင် expand လုပ်လို့ရမရ စစ်ဆေးခြင်း
static bool	is_expandable(char *v, int i, bool sq)
{
	if (v[i] != '$' || sq || !v[i + 1])
		return (false);
	if (ft_isalnum(v[i + 1]) || v[i + 1] == '_' || v[i + 1] == '?')
		return (true);
	return (false);
}

// String တစ်ခုလုံးရှိ variables များကို Expansion လုပ်ခြင်း
char	*expand_status(char *val, t_exec_params *params)
{
	char	*res;
	int		i;
	bool	q[3]; // 0: sq, 1: dq, 2: reached_quotes

	res = ft_strdup("");
	i = 0;
	ft_memset(q, 0, 3 * sizeof(bool));
	while (val && val[i])
	{
		if ((val[i] == '\'' && !q[1]) || (val[i] == '"' && !q[0]))
			handle_status_quotes(val[i], q, &i);
		else if (is_expandable(val, i, q[0])) // $Expansion တွေ့လျှင်
			res = apply_expansion(res, handle_expansion(val, &i, params, q[1]));
		else if (val[i] == '*' && !q[0] && !q[1]) // Unquoted * တွေ့လျှင်
		{
			res = append_char(res, '\2'); // Execution-time globbing အတွက် \2
			i++;
		}
		else
			res = append_char(res, val[i++]);
	}
	if (res[0] == '\0' && !q[2]) // quote မပါဘဲ $NON လိုမျိုး အလွတ်ထွက်လျှင်
	{
		free(res);
		return (ft_strdup("\1")); // Token မပျောက်အောင် \1 marker သုံးသည်
	}
	return (res);
}

**မှတ်ချက်:** Norminette စည်းကမ်း (၅ ဖိုင်လျှင် ၅ function) ပြည့်မီရန် `toggle_quotes`, `is_expandable`, `handle_status_quotes`, `apply_expansion` နှင့် `append_char` တို့ကို `src/parsing/expand_helpers.c` သို့ ပြောင်းရွှေ့ထားပါသည်။
```

---

## ၄။ `src/execution/executor.c` (Execution Core)

```c
#include "minishell.h"

// Variable Assignment (ဥပမာ A=B) ကို ကိုင်တွယ်ခြင်း
static int	handle_assignment(t_command *cmd, char ***envp, int *last_status)
{
	char	*equal;
	char	*key;
	char	*val;

	if (!cmd->args || !cmd->args[0])
		return (0);
	equal = ft_strchr(cmd->args[0], '=');
	if (equal && equal > cmd->args[0] && !cmd->args[1]) // KEY=VALUE ပုံစံ
	{
		key = ft_substr(cmd->args[0], 0, equal - cmd->args[0]);
		val = ft_strdup(equal + 1);
		set_env(envp, key, val); // Environment ထဲထည့်သည်
		free(key);
		free(val);
		return (*last_status = 0, 1);
	}
	return (0);
}

// Single command (Builtin) ကို fork မဆင့်ဘဲ run ခြင်း (cd, export, etc)
static void	handle_single_cmd(t_command *cmd, char ***envp, int *last_status)
{
	int	saved_stdout;
	int	saved_stdin;

	saved_stdout = dup(STDOUT_FILENO); // လက်ရှိ stdout သိမ်းသည်
	saved_stdin = dup(STDIN_FILENO);   // လက်ရှိ stdin သိမ်းသည်
	*last_status = handle_redirections(cmd); // redirections များ လုပ်သည်
	if (*last_status == 0)
		run_single_builtin(cmd, envp, last_status); // Builtin ကို လက်ရှိ process မှာ run သည်
	dup2(saved_stdout, STDOUT_FILENO); // မူလ stdout ပြန်ထားသည်
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
}

// Command node တစ်ခုချင်းစီကို ဘယ်လို run မလဲ ဆုံးဖြတ်ခြင်း
static void	execute_command_node(t_command *curr, char ***envp,
				int *last_status)
{
	expand_cmd_wildcards(curr); // Execution အချိန်ရောက်မှ Wildcards (\2) ကို ဖြေရှင်းသည်
	if (curr->sub_cmd && !curr->next) // (cmd) ပုံစံ Subshell ဖြစ်လျှင်
		handle_process_loop(curr, envp, last_status); 
	else if (!curr->args && curr->redirs && !curr->next) // Redirection-only (> file)
		handle_single_cmd(curr, envp, last_status);
	else if (curr->args && curr->args[0] && !curr->next
		&& is_builtin(curr->args[0])) // Single Builtin (cd, etc)
	{
		if (!handle_assignment(curr, envp, last_status))
			handle_single_cmd(curr, envp, last_status);
	}
	else if ((curr->args && curr->args[0]) || curr->redirs || curr->next
		|| curr->sub_cmd) // Pipe သို့မဟုတ် Binary execute လုပ်ရန်
	{
		if (curr->args && curr->args[0]
			&& handle_assignment(curr, envp, last_status))
			(void)0;
		else
			handle_process_loop(curr, envp, last_status); // Fork လမ်းကြောင်း
	}
}

// Command list တစ်ခုလုံးရှိ && နှင့် || logic များကို handle လုပ်ခြင်း
void	execute_commands(t_command *cmd, char ***envp, int *last_status)
{
	t_command		*curr;
	t_command		*next_p;
	t_command		*stored;
	t_logical_op	op;

	curr = cmd;
	while (curr)
	{
		next_p = curr;
		while (next_p && next_p->logic == LOGIC_NONE && next_p->next)
			next_p = next_p->next; // Pipe များကို ကျော်ပြီး logical operator ရှာသည်
		stored = next_p->next;
		op = next_p->logic;
		next_p->next = NULL; // လက်ရှိ logical block ကို ခွဲထုတ်ပြီး run သည်
		execute_command_node(curr, envp, last_status);
		next_p->next = stored;
		if ((op == LOGIC_AND && *last_status != 0) // && ခံပြီး ရှေ့က ရှုံးလျှင် ရပ်သည်
			|| (op == LOGIC_OR && *last_status == 0)) // || ခံပြီး ရှေ့က အောင်မြင်လျှင် ရပ်သည်
			break ;
		curr = stored;
	}
}
```

---

## ၅။ `src/parsing/wildcards_exec.c` (Execution-time Globbing)

**နားလည်ရန်:** ဤဖိုင်သည် `\2` marker ပါသော text များကို တကယ် globbing (ဥပမာ `file*` -> `file1 file2`) လုပ်ဆောင်သည့်နေရာ ဖြစ်သည်။

```c
// Argument တစ်ခု Globbing လုပ်ပြီးလျှင် စုစုပေါင်း ဘယ်နှစ်ခုထွက်လာမလဲ ရေတွက်သည်
static int	count_expanded(char **args)
{
	int		i;
	int		c;
	char	*tmp;
	t_token	*list;
	t_token	*curr;

	i = -1;
	c = 0;
	while (args[++i])
	{
		if (ft_strchr(args[i], '\2')) // \2 marker ပါလျှင်
		{
			tmp = ft_strdup(args[i]);
			while (ft_strchr(tmp, '\2'))
				*ft_strchr(tmp, '\2') = '*'; // Globbing function အတွက် * ပြန်ပြောင်းသည်
			list = expand_wildcard(tmp); // တကယ် glob လုပ်သည်
			curr = list;
			while (curr && ++c)
				curr = curr->next;
			free_tokens(list);
			free(tmp);
		}
		else
			c++;
	}
	return (c);
}

// Command node တစ်ခုလုံးရှိ arguments နှင့် redirections များရှိ \2 များကို ဖြေရှင်းသည်
void	expand_cmd_wildcards(t_command *cmd)
{
	char	**new_args;
	t_redir	*r;
	t_token	*list;
	int		i;

	if (cmd->args) // Arguments များအတွက်
	{
		new_args = get_expanded_args(cmd->args);
		i = -1;
		while (cmd->args[++i])
			free(cmd->args[i]);
		free(cmd->args);
		cmd->args = new_args;
	}
	r = cmd->redirs; // Redirections များအတွက်
	while (r)
	{
		if (r->filename && ft_strchr(r->filename, '\2'))
		{
			if (count_wildcard_matches(r->filename) > 1) // Match ၁ ခုထက်ပိုလျှင်
			{
				free(r->filename);
				r->filename = ft_strdup(""); // Ambiguous redirect error ပြအောင် အလွတ်လုပ်သည်
			}
			else
			{
				list = expand_wildcard(r->filename); // ၁ ခုပဲဆိုလျှင် expansion လုပ်သည်
				if (list)
				{
					free(r->filename);
					r->filename = ft_strdup(list->value);
					free_tokens(list);
				}
			}
		}
		r = r->next;
	}
	if (cmd->sub_cmd)
		expand_cmd_wildcards(cmd->sub_cmd); // Subshell အတွက်ပါ recursive လုပ်သည်
	if (cmd->next)
		expand_cmd_wildcards(cmd->next); // Pipe command များအတွက်ပါ ဆက်သွားသည်
}
```

---

## ၆။ `src/execution/exec_utils.c` (Process & Pathing)

```c
// Child process က သေဆုံးလျှင် စောင့်ဆိုင်းပြီး exit status ယူခြင်း
void	wait_for_children(pid_t last_pid, int *last_status)
{
	int		status;
	pid_t	pid;

	pid = wait(&status);
	while (pid > 0)
	{
		if (pid == last_pid) // Pipe line ရဲ့ နောက်ဆုံး command ဖြစ်လျှင်
		{
			if (WIFEXITED(status))
				*last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				*last_status = 128 + WTERMSIG(status); // Signal ကြောင့်သေလျှင်
		}
		pid = wait(&status);
	}
}

// Child process အတွင်း ပတ်ဝန်းကျင်ပြင်ဆင်ပြီး command ကို run ခြင်း
void	child_process(t_command *cmd, t_exec_params *params)
{
	if (params->prev_fd != -1) // ရှေ့က pipe ရှိလျှင် input အဖြစ်ယူသည်
	{
		dup2(params->prev_fd, STDIN_FILENO);
		close(params->prev_fd);
	}
	if (params->p_fd[1] != -1) // နောက်မှာ pipe ရှိလျှင် output အဖြစ်ပို့သည်
	{
		dup2(params->p_fd[1], STDOUT_FILENO);
		close(params->p_fd[1]);
	}
	if (params->p_fd[0] != -1)
		close(params->p_fd[0]);
	if (handle_redirections(cmd)) // Files redirection လုပ်သည်
		exit(1);
	if (cmd->sub_cmd) // Subshell ဖြစ်ခဲ့လျှင် recursive ခေါ်သည်
	{
		execute_commands(cmd->sub_cmd, params->envp, params->last_status);
		exit(*(params->last_status));
	}
    // Builtin သို့မဟုတ် Binary execute လုပ်သည်
	if (cmd->args && is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, params->envp, params->last_status));
	if (cmd->args)
		run_command(cmd, params->envp);
	exit(0);
}
```

---


---

## ၇။ `src/parsing/lexer.c` (Tokenization)

```c
// Operator symbols (|, &, <, >, (, )) များကို ကိုင်တွယ်ခြင်း
int	handle_symbol(char *line, int i, t_token **head)
{
	int	res;

	// && , || , ( , ) တို့ကို အရင်စစ်သည်
	res = handle_logical_or_paren(line, i, head);
	if (res != 0)
		return (res);
	if (line[i] == '|') // Pipe တွေ့လျှင်
	{
		append_token(head, new_token(ft_strdup("|"), TOKEN_PIPE));
		return (i + 1);
	}
	if (line[i] == ';') // Semicolon တွေ့လျှင်
	{
		append_token(head, new_token(ft_strdup(";"), TOKEN_SEMI));
		return (i + 1);
	}
	if (line[i] == '<') // Redirection In တွေ့လျှင်
		return (handle_red_in(line, i, head));
	if (line[i] == '>') // Redirection Out တွေ့လျှင်
		return (handle_red_out(line, i, head));
	return (i + 1);
}

// စာသားကွင်းဆက်တစ်ခုလုံးကို Token များအဖြစ် ခွဲထုတ်ခြင်း
t_token	*tokenize(char *line, t_exec_params *params)
{
	t_token	*head;
	int		i;

	head = NULL;
	i = 0;
	while (line[i])
	{
		if (is_whitespace(line[i])) // Space ဆိုလျှင်ကျော်သည်
			i++;
		else if (ft_strchr("|&<>()", line[i])) // Operator တွေ့လျှင်
			i = handle_symbol(line, i, &head);
		else // Word (ls, -l, etc) တွေ့လျှင်
		{
			i = handle_word(line, i, &head, params);
			if (i == -1) // Quote မပိတ်ခဲ့လျှင်
			{
				ft_putendl_fd("minishell: syntax error: unclosed quote", 2);
				*params->last_status = 2; // Status 2 ဖြင့် error ပြသည်
				free_tokens(head);
				return (NULL);
			}
		}
	}
	return (head);
}
```

---

## ၈။ `src/parsing/parser.c` (Command Tree Building)

```c
// Token list မှ Command list (t_command) သို့ ပြောင်းလဲတည်ဆောက်ခြင်း
t_command	*parse_tokens(t_token *tokens)
{
	t_token			*tmp;
	t_command		*head;
	t_command		*curr;

	if (!tokens)
		return (NULL);
	head = new_command(); // list အသစ်စသည်
	curr = head;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_L_PAREN) // ( တွေ့လျှင် subshell အဖြစ်ကိုင်တွယ်သည်
		{
			tmp = handle_subshell(tmp, &curr);
            // subshell ပြီးလျှင် pipe/logical ဆက်မဆက် စစ်သည်
			if (tmp && (tmp->type == TOKEN_PIPE || tmp->type == TOKEN_AND
					|| tmp->type == TOKEN_OR))
				continue ;
			if (!tmp)
				break ;
		}
		tmp = handle_token(tmp, &curr); // သာမန် word/redir/pipe များကို ထည့်သည်
		if (!tmp)
			break ;
	}
	return (head);
}

**မှတ်ချက်:** `;`, `&&`, `||` နှင့် `|` တို့ကို `handle_token` တွင် `LOGIC_SEQ`, `LOGIC_AND`, `LOGIC_OR` အဖြစ် သတ်မှတ်ပြီး command node အသစ်များ တည်ဆောက်ပါသည်။

```

---

## ၉။ `src/builtins/builtins.c` (Standard Commands)

```c
// echo command implementation
int	ft_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (args[1] && ft_strncmp(args[1], "-n", 3) == 0) // -n option စစ်သည်
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1); // စာလုံးကြား space ခြားသည်
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1); // အဆုံးသတ် newline
	return (0);
}

// exit command implementation
int	ft_exit(char **args, int *last_status)
{
	int	code;

	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", 1);
	code = *last_status;
	if (!args[1]) // argument မပါလျှင် variable သုံးသည်
		exit((unsigned char)code);
	if (!is_all_digits(args[1])) // ဂဏန်းမဟုတ်လျှင် ၂၅၅ ဖြင့်ထွက်သည်
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		exit(255);
	}
	if (args[2]) // argument များလွန်းလျှင် မထွက်ပါ
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (1);
	}
	exit((unsigned char)ft_atoll(args[1])); // code ဖြင့် ထွက်သည်
	return (0);
}
```

---


---

## ၁၀။ `src/utils/env_utils.c` (Environment Management)

```c
// System Environment ကို clone ကူးယူခြင်း
char	**copy_env(char **envp)
{
	char	**new_env;
	int		count;

	count = 0;
	while (envp[count])
		count++;
	new_env = (char **)malloc(sizeof(char *) * (count + 1));
	if (!new_env) return (NULL);
	int i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(envp[i]); // String တစ်ခုချင်းစီ ကူးယူသည်
		i++;
	}
	new_env[i] = NULL; // NULL ဖြင့် အဆုံးသတ်သည်
	return (new_env);
}

// Environment ထဲမှ value ကို ရှာဖွေခြင်း (ဥပမာ USER ရိုက်လျှင် hko-ko ပြန်ပေးသည်)
char	*get_env_value(char **envp, char *key)
{
	int	i = 0;
	int	key_len = ft_strlen(key);
	while (envp[i])
	{
		// "KEY=" နဲ့ စမစ စစ်ဆေးသည်
		if (ft_strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
			return (envp[i] + key_len + 1); // value ရှိရာ နေရာကို ပြန်ပေးသည်
		i++;
	}
	return (NULL);
}

// Variable အသစ်ထည့်ခြင်း သို့မဟုတ် အဟောင်းကို update လုပ်ခြင်း
void	set_env(char ***envp, char *key, char *value)
{
	// ၁။ ရှိပြီးသားလား ရှာသည်
	// ၂။ ရှိလျှင် value အသစ်ကို အစားထိုးသည်
	// ၃။ မရှိလျှင် environment array ကို malloc အသစ်ဖြင့် ချဲ့ပြီး အဆုံးမှာ ထည့်သည်
}
```

---

## ၁၁။ `src/builtins/builtins_utils.c` (Helpers & Formatting)

```c
// export command အတွက် environment variables များကို alphabetical order ဖြင့် format လုပ်ထုတ်ခြင်း
void	print_export(char **envp)
{
	int		i = 0;
	char	*eq;

	while (envp[i])
	{
		if (ft_strncmp(envp[i], "_", 1) == 0) { i++; continue; } // _ ကို skip လုပ်သည်
		ft_putstr_fd("declare -x ", STDOUT_FILENO); // bash format prefix
		eq = ft_strchr(envp[i], '=');
		if (eq)
		{
			write(STDOUT_FILENO, envp[i], eq - envp[i] + 1); // KEY=
			ft_putstr_fd("\"", STDOUT_FILENO);
			ft_putstr_fd(eq + 1, STDOUT_FILENO); // VALUE
			ft_putendl_fd("\"", STDOUT_FILENO);
		}
		else
			ft_putendl_fd(envp[i], STDOUT_FILENO);
		i++;
	}
}

// exit code အတွက် numeric overflow စစ်ဆေးခြင်း
int	is_all_digits(char *str)
{
    // ဂဏန်းတွေချည်းပဲလား စစ်သည်
    // Long Long limit (9223372036854775807) ကျော်မကျော် စစ်သည်
}
```

---

## ၁၂။ `src/builtins/env_builtins.c` (Export & Unset Logic)

```c
// export command ဖြင့် variable အသစ်ထည့်ခြင်း
static int	export_one(char *arg, char ***envp)
{
	char	*key;
	char	*equal = ft_strchr(arg, '=');

    // ၁။ Identifier (နမည်) က alphanumeric ဖြစ်မဖြစ် စစ်သည်
	// ၂။ Validation အောင်လျှင် set_env ဖြင့် ထည့်သည်
}

// unset command ဖြင့် variable ကို ဖြတ်ထုတ်ခြင်း
static void	unset_var(char ***envp, char *key)
{
    // ၁။ key ကို ပတ်ရှာသည်
	// ၂။ တွေ့လျှလျှင် ထိုနေရာကို free လုပ်ပြီး array ထဲမှ ကျန်တာများကို ရှေ့တိုးသည်
}
```

---

## ၁၃။ `src/execution/builtin_executor.c` (Builtin Routing)

```c
// ပေးထားသော command သည် shell ရဲ့ builtin ဟုတ်မဟုတ် စစ်ဆေးခြင်း
int	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0) return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0) return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0) return (1);
	if (ft_strncmp(cmd, "export", 7) == 0) return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0) return (1);
	if (ft_strncmp(cmd, "env", 4) == 0) return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0) return (1);
	return (0);
}

// Builtin command ကို သက်ဆိုင်ရာ function ပေါင်းပေးခြင်း
int	execute_builtin(t_command *cmd, char ***envp, int *last_status)
{
    // ဥပမာ echo ဆိုလျှင် ft_echo ကို ခေါ်သည်
}
```

---

## ၁၄။ `src/execution/exec_loop.c` (Pipe & Process Loop)

```c
// Command များကို pipe ခံပြီး တစ်ခုချင်းစီ fork လုပ် run သည့် loop
void	handle_process_loop(t_command *cmd, char ***envp, int *last_status)
{
	t_exec_params	p;
	pid_t			pid;

	while (cmd)
	{
		setup_pipe(cmd, p.p_fd); // Pipe များ တည်ဆောက်သည်
		pid = fork();            // Process အသစ် ခွဲထုတ်သည်
		if (pid == 0)            // Child process ထဲရောက်လျှင်
			child_process(cmd, &p);
		cleanup_parent(&p, &cmd); // Parent က pipe များကို ရှင်းလင်းသည်
	}
	wait_for_children(last_pid, last_status); // အကုန်ပြီးအောင်စောင့်သည်
}
```

---


---

## ၁၅။ `src/execution/redirection.c` (FD Handling)

```c
// Redirection (<, >, >>) များအတွက် file descriptor (fd) များဖွင့်ခြင်း
int	handle_redirections(t_command *cmd)
{
	t_redir	*redir = cmd->redirs;

	while (redir)
	{
        // ၁။ Ambiguous Redirect (ဥပမာ > $VAR မှာ $VAR မရှိခြင်း) ကို စစ်သည်
		if (check_ambiguous(redir)) return (1);

        // ၂။ Redirection အမျိုးအစားအလိုက် file ကို ဖွင့်သည်
		if (redir->type == REDIR_IN)      // < file
			handle_in(redir);
		else if (redir->type == REDIR_HEREDOC) // << EOF
		{
			if (handle_heredoc(redir)) // Heredoc ကို process လုပ်သည်
				return (1);
		}
		redir = redir->next;
	}
	return (0);
}

// Heredoc ကို stdin သို့ pipe မှတစ်ဆင့် ပို့ပေးခြင်း
static void	read_heredoc(int fd, char *delim)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delim, ft_strlen(delim) + 1) == 0)
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	free(line);
}

```

---

## ၁၆။ `src/execution/path_utils.c` (System Lookup)

```c
// PATH environment ထဲမှ binary file များကို လိုက်ရှာခြင်း (ဥပမာ ls က /bin/ls မှာ ရှိနေခြင်း)
char	*find_command_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path_env = get_env_value(envp, "PATH"); // PATH ရှာသည်

	if (!path_env) return (NULL);
	paths = ft_split(path_env, ':'); // /bin:/usr/bin ကို ခွဲထုတ်သည်
    
    // တစ်ခုချင်းစီ ထဲမှာ ls ဆိုတာ ရှိလား access() နဲ့ လမ်းပတ်စစ်သည်
	res = try_paths(paths, cmd);
	free_split(paths);
	return (res);
}
```

---

## ၁၇။ `src/parsing/lexer_utils.c` (Scanning Helpers)

```c
// စာသားတစ်ခုရဲ့ အဆုံး (Word end) ကို ရှာဖွေခြင်း
int	get_word_end(char *line, int i)
{
    // Space တွေ့လျှင် သို့မဟုတ် operator (| < >) တွေ့လျှင် ရပ်သည်
    // Quote (' ") ထဲမှာ ရှိနေလျှင် အဆုံးအထိ ကျော်ဖတ်သည်
}

// Token ထဲသို့ word ထည့်ရာတွင် Wildcard ရှိမရှိပါ စစ်ဆေးသည်
static void	add_word_token(t_token **head, char *word, char *expanded)
{
    // Redirection target မှာ wildcard match များလွန်းလျှင် Ambiguous ဖြစ်စေသည်
	append_token(head, new_token(expanded, TOKEN_WORD));
}
```

---

## ၁၈။ `src/parsing/parser_redir.c` (Validation & Redir Parsing)

```c
// ရိုက်လိုက်သော command ၏ syntax မှန်မမှန် စစ်ဆေးခြင်း
int	validate_syntax(t_token *tokens)
{
    // ၁။ ( ) အဖွင့်အပိတ် ညီမညီ စစ်သည်
    // ၂။ Pipe/Logical တွေ တစ်ခုပြီးတစ်ခု ရှေ့ဆင့်နောက်ဆင့် ဖြစ်နေလား စစ်သည် (ဥပမာ ls ||| grep)
    // ၃။ Redirection ပြီးလျှင် file name ပါလား စစ်သည်
}
```

---

## ၁၉။ `src/parsing/token_utils.c` (Memory & String Helpers)

```c
// Token list အတွက် memory များကို ရှင်းလင်းခြင်း
void	free_tokens(t_token *tokens)
{
	while (tokens)
	{
		free(tokens->value);
		free(tokens);
	}
}

// Token အသစ်တစ်ခု တည်ဆောက်ခြင်း
t_token	*new_token(char *value, t_token_type type)
{
	token->value = value;
	token->type = type;
	token->next = NULL;
}
```

---


---

## ၂၀။ `src/parsing/parser_utils.c` (CMD Tree Helpers)

```c
// Command သို့ argument တစ်ခုချင်းစီ ပေါင်းထည့်ခြင်း
void	add_argument(t_command *cmd, char *arg)
{
	if (arg && arg[0] == '\1') return ; // \1 marker (empty expansion) ဆိုလျှင် မထည့်ပါ

    // ၁။ cmd->args array တစ်ခု malloc လုပ်သည်
    // ၂။ ရှိပြီးသား argument များကို ကူးထည့်သည်
    // ၃။ အသစ်ကို အဆုံးမှာ ထည့်သည် (NULL-terminated)
}
```

---

## ၂၁။ `src/parsing/wildcards.c` (Matching Core)

```c
// လက်ရှိ folder ထဲရှိ file များထဲမှ pattern နှင့် ကိုက်ညီတာများကို ရှာဖွေခြင်း
t_token	*expand_wildcard(char *pattern)
{
	DIR *dir = opendir("."); // လက်ရှိ directory ကို ဖွင့်သည်
	struct dirent *e;

	while ((e = readdir(dir)))
	{
		// fnmatch() function သုံးပြီး pattern နဲ့ file နမည် တိုက်စစ်သည်
		if (fnmatch(pattern, e->d_name, 0) == 0)
			append_token(&head, new_token(ft_strdup(e->d_name), TOKEN_WORD));
	}
	closedir(dir);
	sort_strings(matches, c); // A-Z အစီအစဉ်ဖြင့် စီသည်
	return (head);
}
```

---

## ၂၂။ `src/signals.c` (Signal Management)

```c
// Ctrl+C သို့မဟုတ် Ctrl+\ အချက်ပြချက်များကို ကိုင်တွယ်ခြင်း
void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_received = 1;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);  // Ctrl+C handle လုပ်သည်
	signal(SIGQUIT, SIG_IGN);       // Ctrl+\ ကို ignore လုပ်သည်
}
```

---

## ၂၃။ `src/parsing/expand_helpers.c` (Expansion Helpers)

ဤဖိုင်တွင် `expand_utils.c` မှ ဖယ်ထုတ်ထားသော helper function ၅ ခု ပါဝင်သည်။

- `toggle_quotes`: Single/Double quotes အဖွင့်အပိတ်ကို ပြောင်းလဲပေးသည်။
- `apply_expansion`: Variable တန်ဖိုးကို လက်ရှိ result string နှင့် ပေါင်းစည်းပေးပြီး memory ရှင်းလင်းပေးသည်။
- `is_expandable`: Character သည် expansion လုပ်ရန် သင့်တော်သော `$` ဟုတ်မဟုတ် စစ်ဆေးသည်။
- `handle_status_quotes`: Quote များတွေ့လျှင် quote state ကို update လုပ်ပြီး index ကို တိုးပေးသည်။
- `append_char`: String တစ်ခု၏ အဆုံးတွင် character တစ်လုံး ပေါင်းထည့်ပေးသည်။

---

## ၂၄။ `src/parsing/wildcard_helpers.c` (Wildcard Helpers)

ဤဖိုင်တွင် Wildcard logic အတွက် လိုအပ်သော helper function ၅ ခု ပါဝင်သည်။

- `has_unquoted_wildcard`: String ထဲတွင် quote မပါသော `*` ပါမပါ စစ်ဆေးသည်။
- `has_unquoted_var`: String ထဲတွင် quote မပါသော `$` ပါမပါ စစ်ဆေးသည်။
- `get_expanded_count`: Wildcard pattern တစ်ခုသည် file ဘယ်နှစ်ခုနှင့် match ဖြစ်သည်ကို ရေတွက်သည်။
- `clean_marker`: String ထဲရှိ `\2` marker များကို မူလ `*` သို့ ပြန်ပြောင်းပေးသည်။
- `fill_wildcard_list`: Pattern နှင့် match ဖြစ်သော file နမည်များကို argument list ထဲသို့ ဖြည့်သွင်းပေးသည်။

---

## ၂၅။ `src/parsing/token_utils.c` (Updated)

ဤဖိုင်သည် Token management အတွက် ဖြစ်ပြီး `sort_strings` ကိုလည်း ဤနေရာသို့ ပြောင်းရွှေ့ထားသည်။

---

## နိဂုံး (Conclusion)

ဤ Minishell ပရောဂျက်သည် Unix-like shell တစ်ခု၏ အခြေခံလုပ်ဆောင်ချက်များဖြစ်သော **Lexing (စာသားခွဲထုတ်ခြင်း)**, **Parsing (အမိန့်တော်များတည်ဆောက်ခြင်း)** နှင့် **Execution (အလုပ်လုပ်စေခြင်း)** တို့ကို အပြည့်အစုံ လွှမ်းခြုံထားပါသည်။

> [!TIP]
> **Subshell** နှင့် **Execution-time Wildcards** ကဲ့သို့သော အဆင့်မြင့် features များသည် memory နှင့် process စီမံခန့်ခွဲမှုကို ပိုမိုနားလည်စေရန် အထောက်အကူပြုပါလိမ့်မည်။

---
*Generated with exhaustive line-by-line details for educational purposes.*

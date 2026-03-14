# Minishell

As beautiful as a shell.

## 1. Introduction
Minishell is a 42 school project about creating a simple shell. It aims to replicate basic functionalities of the bash shell, allowing users to understand the internal mechanisms of process creation, file descriptor manipulation, and signal handling.

## 2. Compilation and Execution

### Compilation
The project includes a Makefile providing the required rules: `all`, `clean`, `fclean`, and `re`.
To compile the shell, simply run:
```bash
make
```

### Execution
After compilation, execute the binary:
```bash
./minishell
```

## 3. Supported Features

### Core Operations
* Display a prompt when waiting for a new command.
* Have a working history.
* Search and launch the right executable (based on the PATH variable or using a relative or an absolute path).

### Parsing and Quotation
* Handles single quotes `\'` which prevents the shell from interpreting the meta-characters in the quoted sequence.
* Handles double quotes `\"` which prevents the shell from interpreting the meta-characters in the quoted sequence except for `$` (dollar sign).

### Redirections and Pipes
* `<` redirects input from a file.
* `>` redirects output to a file (truncate).
* `>>` redirects output to a file (append).
* `<<` (heredoc) reads input until a line containing the given delimiter is seen.
* `|` pipe connects the output of each command in the pipeline to the input of the next command via a pipe.

### Environment Management
* Handles environment variables `$` which expand to their values.
* Handles `$?` which expands to the exit status of the most recently executed foreground pipeline.

### Signals
* `ctrl-C` displays a new prompt on a new line.
* `ctrl-D` exits the shell.
* `ctrl-\` does nothing.

### Builtins
The shell implements the following basic builtins:
* `echo` with option `-n`
* `cd` with only a relative or absolute path
* `pwd` with no options
* `export` with no options
* `unset` with no options
* `env` with no options or arguments
* `exit` with no options (and with specific numeric arguments if within the bonus/extended logic).

## 4. Architecture

* **Lexer**: Reads the raw input line and breaks it down into individual tokens (words, pipes, redirections) while respecting quoting rules.
* **Parser**: Groups the tokens into command structures representing sequences of simple commands separated by pipes, complete with their respective argument lists and redirection sets.
* **Executor**: Iterates through the command structures. It sets up pipes and file descriptors using `pipe()` and `dup2()`, forks new child processes with `fork()`, and replaces the current image with the new process via `execve()`. Builtins modifying the shell environment are handled directly in the parent process.

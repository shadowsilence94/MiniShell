# Minishell - A robust re-implementation of a Unix shell

## Description
Minishell is a comprehensive project within the 42 School curriculum that requires building a fully functional shell from the ground up using C. The project aims to replicate the core functionalities of bash, providing a deep dive into process management, file description manipulation, and signal handling. By developing this application, I gained extensive experience in system-level programming and the intricacies of the POSIX standard.

The primary objective was to create a robust execution engine capable of handling command pipelines, logical operators, and complex redirections. Beyond simple command execution, the project involved implementing a suite of built-in commands (echo, cd, pwd, export, unset, env, exit) and ensuring they behave identically to their bash counterparts. This required meticulous attention to edge cases, error messaging, and precise exit status management.

**Key Challenge:** The most significant technical hurdle was designing a thread-safe and leak-free recursive parser that could correctly interpret nested logic (such as subshells and parenthesized expressions) without overflowing the stack or losing track of file descriptors in deep process trees.

## Technologies & Concepts
- **Language:** C (Standard C99)
- **Process Management:** Forking, waitpid, process groups.
- **Inter-process Communication:** Pipes, anonymous and named (via redirections).
- **Environment Handling:** Variable expansion, static and dynamic environment management.
- **Signal Handling:** Non-interactive and interactive signal masking (SIGINT, SIGQUIT).
- **Data Structures:** Doubly linked lists, abstract syntax trees (AST), hash maps for environment storage.

## How It Works
### Data Flow:
```
┌─────────┐      ┌─────────┐      ┌─────────────┐      ┌─────────────┐
│  Input  │─────►│  Lexer  │─────►│   Parser    │─────►│  Executor   │
└─────────┘      └─────────┘      └─────────────┘      └─────────────┘
     ^               |                 |                   |
     |               v                 v                   v
     └───────────────┴─────────────────┴───────────────────┘
                      System Environment & Global Status ($?)
```

## Architecture
### Main Command Structure:
```c
/* Command structure for representing simple and complex command nodes */
typedef struct s_command
{
	char				**args;       // Array of strings (cmd + flags)
	char				*cmd_path;    // Full path resolved via PATH
	t_redir				*redirs;      // Linked list of input/output files
	t_logical_op		logic;        // Logic flag (AND, OR, PIPELINE)
	struct s_command	*sub_cmd;     // Nested command for subshells
	struct s_command	*next;        // Next link in the command list
}	t_command;

/* State structure for expansion and interpolation */
typedef struct s_expand
{
	char	*result;
	int		i;
	int		j;
}	t_expand;
```

## Source Code Structure
```
minishell/
├── Makefile                # Multi-target build script
├── include/
│   └── minishell.h         # Global header with definitions
├── libft/                  # 42 standard library extension
├── src/                    # Source implementation
│   ├── main.c              # REPL and top-level logic
│   ├── parsing/            # Lexing, syntax trees, and expansion
│   ├── execution/          # Process forking, pipes, and redirs
│   ├── builtins/           # Internal bash-compliant commands
│   ├── utils/              # Memory management and environment
│   └── signals.c           # Execution-mode signal handling
└── README.md               # Project documentation
```

## Build/Run/Test Instructions
### Building the Project
The project uses a standard Makefile for compilation.
```bash
make            # Compiles the final minishell binary
make clean      # Removes object files
make fclean     # Removes objects and the binary
make re         # Complete cleanup and rebuild
```

### Running the Shell
```bash
./minishell
```

### Testing Examples
```bash
# Basic Pipeline
ls -l | grep ".c" | wc -l

# Logical Operators with Redirections
ls nonexistent && echo "Found" || echo "Not Found" > error.log

# Environment Variable Expansion
export TEST="Hello World"
echo $TEST
```

## Technical Challenges & Solutions
<details>
<summary><strong>Recursive Execution & File Descriptor Management</strong></summary>
**Problem:** Deep pipelines (e.g., `cat /dev/urandom | head -c 100 | wc -c`) often lead to file descriptor exhaustion or "hanging" processes if pipe ends aren't closed correctly in both the parent and child.
**Solution:** Refactored the execution loop to follow a strict "Close in Parent" policy immediately after forking, ensuring that file descriptors are never inherited unnecessarily by subsequent child processes.
</details>

<details>
<summary><strong>Non-blocking Signal Handling</strong></summary>
**Problem:** Correctly distinguishing between when the shell is waiting for a prompt versus when it is waiting for a child process to terminate (affecting how SIGINT is handled).
**Solution:** Implemented temporary signal handlers that are context-aware, swapping the global signal state before and after process forking to ensure the prompt isn't interrupted by noise from child processes.
</details>

## Useful Links
- [POSIX Shell Specification](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)
- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/)
- [The Linux Command Line (Book)](https://linuxcommand.org/tlcl.php)

## License
This project is released under the 42 School Academic Unlicense. It is intended for educational purposes only.

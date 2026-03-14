# Minishell Test Suite

Here is a list of commands to verify the functionality of your Minishell. run these inside `./minishell`.

## 1. Simple Commands
Basic execution of external programs.
```bash
/bin/ls
ls
ls -l
ls -l -a
pwd
whoami
```

## 2. Arguments & Quotes
Testing argument parsing and quote handling.
```bash
echo Hello World
echo "Hello World"
echo 'Hello World'
echo "Hello      World"      # Should preserve spaces
echo 'Hello      World'      # Should preserve spaces
echo "Hello 'World'"         # Nested quotes
echo 'Hello "World"'         # Nested quotes
```

## 3. Pipes (`|`)
Testing data flow between processes.
```bash
ls | cat
ls -l | grep min
ls -l | grep min | cat -e
cat | cat | ls           # Should list files instantly (async execution check)
echo "test" | wc -l | cat -e
ls | grep "\.c" | wc -l | awk '{print $1}' | cat -e   # Heavy piping test
```

## 4. Redirections
Testing Input/Output redirection.

### Output (`>`)
```bash
ls > out.txt
cat out.txt
echo "New Content" > out.txt
cat out.txt              # Should show only "New Content"
```

### Append (`>>`)
```bash
echo "Append Line 1" >> append.txt
echo "Append Line 2" >> append.txt
cat append.txt           # Should show both lines
```

### Input (`<`)
```bash
cat < out.txt
grep "New" < out.txt
```

### Heredoc (`<<`)
*Note: Depending on implementation status*
```bash
cat << EOF
line 1
line 2
EOF
```

## 5. Builtins
Testing shell built-in commands.

### `echo`
```bash
echo -n Hello
echo World
```

### `cd` & `pwd`
```bash
pwd
cd ..
pwd
cd minishell_final
pwd
```

### `env`
```bash
env
```

### `exit`
Testing exit codes and behaviors.
```bash
exit
# Shell should close cleanly

exit 42
# Shell should close, type `echo $?` in bash to verify it returns 42

exit 255
# Shell should close, `echo $?` should be 255

exit abc
# Should print error "numeric argument required" and exit with code 2

exit 1 2 3
# Should print error "too many arguments" and NOT exit

exit 9223372036854775807
# Edge case for max positive long long

exit -9223372036854775808
# Edge case for min negative long long
```

### `$?` (Exit Status)
```bash
echo $?                  # Should print last command exit status
ls /invalid/path
echo $?                  # Should print non-zero (usually 1, 2, or 127 depending on system)
echo $? | cat            # Testing expansion in pipeline
```

## 6. Signals
- **Ctrl-C**: Type some text like `cat`, then press `Ctrl-C`. It should interrupt the process and give a new line.
- **Ctrl-D**: On an empty line, press `Ctrl-D`. It should exit the shell (print `exit`).
- **Ctrl-\**: Run `cat`, then press `Ctrl-\`. It should do nothing (Quit ignored).

## 7. Edge Cases & Errors
```bash
/bin/ls-404              # Command not found, check exit code `$?` (should be 127)
./minishell              # Running shell inside shell (increment SHLVL test)
""                       # Empty quote command
" "                      # Single space command
> out.txt                # Redirection without command (should create file)
< in.txt                 # Invalid input redirection without command if file does not exist
```

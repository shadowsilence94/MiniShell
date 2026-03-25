echo "echo success && echo done" | ./minishell
echo "false && echo never" | ./minishell
echo "false || echo choice" | ./minishell
echo "echo first || echo second" | ./minishell
echo "cat << EOF
hello
EOF" | ./minishell

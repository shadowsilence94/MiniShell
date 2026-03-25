echo "echo hello > file && cat file" | ./minishell
echo "cat << EOF > file2
test_heredoc_to_file
EOF" | ./minishell
cat file2

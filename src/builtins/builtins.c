/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/14 21:10:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (args[1] && ft_strncmp(args[1], "-n", 3) == 0)
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

int	ft_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		return (0);
	}
	return (1);
}

static void	update_dir_env(char ***envp, char *old)
{
	char	cwd[1024];

	if (old[0])
		set_env(envp, "OLDPWD", old);
	if (getcwd(cwd, sizeof(cwd)))
		set_env(envp, "PWD", cwd);
}

int	ft_cd(char **args, char ***envp)
{
	char	*path;
	char	old[1024];

	if (args[1] && args[2])
		return (ft_putendl_fd("minishell: cd: too many arguments", 2), 1);
	path = args[1];
	if (!path)
		path = get_env_value(*envp, "HOME");
	else if (ft_strncmp(path, "-", 2) == 0)
		path = get_env_value(*envp, "OLDPWD");
	if (!path)
		return (ft_putendl_fd("minishell: cd: path not set", 2), 1);
	if (args[1] && ft_strncmp(args[1], "-", 2) == 0)
		ft_putendl_fd(path, STDOUT_FILENO);
	if (!getcwd(old, sizeof(old)))
		old[0] = '\0';
	if (chdir(path) == -1)
		return (perror("cd"), 1);
	return (update_dir_env(envp, old), 0);
}

int	ft_exit(char **args, int *last_status)
{
	int	code;

	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", 1);
	code = *last_status;
	if (!args[1])
		exit((unsigned char)code);
	if (!is_all_digits(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		exit(255);
	}
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (1);
	}
	exit((unsigned char)ft_atoll(args[1]));
	return (0);
}

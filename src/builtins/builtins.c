/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antigravity <antigravity@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by antigravity       #+#    #+#             */
/*   Updated: 2025/12/26 12:00:00 by antigravity      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **args)
{
	int		i;
	int		newline;

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

int	ft_cd(char **args, char ***envp)
{
	char	*path;

	(void)envp; // TODO: Update PWD/OLDPWD in env
	if (!args[1])
	{
		// TODO: invalid connection to HOME env variable
		// path = get_env_value(*envp, "HOME"); 
		// For consistency with simplistic shell, assuming HOME is not handled or handled simply
		return (1); 
	}
	else
		path = args[1];
	if (chdir(path) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	ft_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		ft_putendl_fd(envp[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}

static int	is_all_digits(char *str)
{
	unsigned long long	res;
	int					i;
	int					sign;

	i = 0;
	res = 0;
	sign = 1;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			sign = -1;
	}
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		res = res * 10 + (str[i++] - '0');
		// LLONG_MAX is 9223372036854775807
		if ((sign == 1 && res > 9223372036854775807ULL)
			|| (sign == -1 && res > 9223372036854775808ULL))
			return (0);
	}
	return (1);
}

static long long	ft_atoll(const char *str)
{
	unsigned long long	res;
	int					i;
	int					sign;

	i = 0;
	res = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			sign = -1;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i++] - '0');
	}
	return ((long long)res * sign);
}

int	ft_exit(char **args, int *last_status)
{
	int	code;

	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", 1);
	code = *last_status;
	if (!args[1])
	{
		exit((unsigned char)code);
	}
	if (!is_all_digits(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		exit(2);
	}
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (1);
	}
	exit((unsigned char)ft_atoll(args[1]));
	return (0);
}

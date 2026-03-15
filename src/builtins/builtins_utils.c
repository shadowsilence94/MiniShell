/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2026/03/14 22:25:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_all_digits(char *str)
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
		if ((sign == 1 && res > 9223372036854775807ULL)
			|| (sign == -1 && res > 9223372036854775808ULL))
			return (0);
	}
	return (1);
}

long long	ft_atoll(const char *str)
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

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	increment_shlvl(char ***envp)
{
	char	*shlvl;
	int		val;
	char	*new_val;

	shlvl = get_env_value(*envp, "SHLVL");
	if (!shlvl)
		set_env(envp, "SHLVL", "1");
	else
	{
		val = ft_atoi(shlvl) + 1;
		new_val = ft_itoa(val);
		set_env(envp, "SHLVL", new_val);
		free(new_val);
	}
}

void	print_export(char **envp)
{
	int		i;
	char	*eq;

	i = 0;
	while (envp[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		eq = ft_strchr(envp[i], '=');
		if (eq)
		{
			write(STDOUT_FILENO, envp[i], eq - envp[i] + 1);
			ft_putstr_fd("\"", STDOUT_FILENO);
			ft_putstr_fd(eq + 1, STDOUT_FILENO);
			ft_putendl_fd("\"", STDOUT_FILENO);
		}
		else
			ft_putendl_fd(envp[i], STDOUT_FILENO);
		i++;
	}
}

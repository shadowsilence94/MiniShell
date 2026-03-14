/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antigrav <antigrav@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 16:35:00 by antigrav       #+#    #+#             */
/*   Updated: 2026/03/14 16:35:00 by antigrav      ###   ########.fr       */
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


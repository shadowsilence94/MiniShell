/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 16:35:00 by hko-ko           #+#    #+#             */
/*   Updated: 2026/03/14 16:35:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static char	*copy_expanded(char *val, char *status_str, int len)
{
	char	*new_val;
	int		i;
	int		j;
	new_val = (char *)malloc(len + 1);
	if (!new_val)
		return (NULL);
	i = 0;
	j = 0;
	while (val[i])
	{
		if (val[i] == '$' && val[i + 1] == '?')
		{
			ft_strlcpy(new_val + j, status_str, len + 1 - j);
			j += ft_strlen(status_str);
			i += 2;
		}

		else
			new_val[j++] = val[i++];
	}

	new_val[j] = '\0';
	return (new_val);
}

char	*expand_status(char *val, int last_status)
{
	char	*new_val;
	char	*status_str;
	int		len;
	if (!ft_strnstr(val, "$?", ft_strlen(val)))
		return (val);
	status_str = ft_itoa(last_status);
	if (!status_str)
		return (val);
	len = ft_strlen(val) + ft_strlen(status_str) - 2;
	new_val = copy_expanded(val, status_str, len);
	free(status_str);
	if (!new_val)
		return (val);
	free(val);
	return (new_val);
}


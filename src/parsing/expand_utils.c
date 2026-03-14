/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2025/12/26 12:00:00 by hko-ko           ###   ########.fr       */
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
			ft_strlcpy(new_val + j, status_str, ft_strlen(status_str) + 1);
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
	char	*status_str;
	char	*new_val;
	int		len;
	int		i;

	status_str = ft_itoa(last_status);
	len = 0;
	i = 0;
	while (val[i])
	{
		if (val[i] == '$' && val[i + 1] == '?')
		{
			len += ft_strlen(status_str);
			i += 2;
		}
		else
		{
			len++;
			i++;
		}
	}
	new_val = copy_expanded(val, status_str, len);
	free(status_str);
	return (new_val);
}

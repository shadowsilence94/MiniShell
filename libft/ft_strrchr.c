/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 19:22:15 by hko-ko            #+#    #+#             */
/*   Updated: 2025/08/29 19:22:17 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;

	i = ft_strlen((char *)s);
	if (!s)
		return (NULL);
	while (i >= 0)
	{
		if ((char)c == s[i])
			return ((char *)&s[i]);
		i--;
	}
	return (NULL);
}

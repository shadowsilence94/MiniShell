/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 19:18:11 by hko-ko            #+#    #+#             */
/*   Updated: 2025/08/29 19:18:13 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*box;
	size_t	i;
	size_t	z;

	i = 0;
	z = 0;
	if (!s1 || !s2)
		return (NULL);
	box = malloc((sizeof(char) * ft_strlen((char *)s1)) + 1
			+ (sizeof(char) * ft_strlen((char *)s2)));
	if (!box)
		return (NULL);
	while (i < ft_strlen((char *)s1))
	{
		box[i] = s1[i];
		i++;
	}
	while (z < ft_strlen((char *)s2))
	{
		box[i] = s2[z];
		z++;
		i++;
	}
	box[i] = '\0';
	return (box);
}

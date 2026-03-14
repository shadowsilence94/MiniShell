/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 19:21:11 by hko-ko           #+#    #+#             */
/*   Updated: 2025/08/29 19:21:13 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	char	*box;

	i = 0;
	if (!s || !f)
		return (NULL);
	box = malloc(sizeof(char) * (ft_strlen((char *)s)) + 1);
	if (!box)
		return (NULL);
	while (s[i] != '\0')
	{
		box[i] = f(i, s[i]);
		i++;
	}
	box[i] = '\0';
	return (box);
}

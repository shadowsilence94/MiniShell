/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2025/12/26 12:00:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	sizefinder(char *s, size_t size)
{
	size_t	i;

	i = 0;
	while ((i < size) && s[i] != '\0')
	{
		i++;
	}
	return (i);
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	z;
	size_t	a;

	a = sizefinder(dst, size);
	z = 0;
	i = sizefinder(dst, size);
	if (!dst || !src)
		return (0);
	while ((i + 1 < size) && src[z] != '\0')
	{
		dst[i] = src[z];
		z++;
		i++;
	}
	if (i != size)
		dst[i] = '\0';
	return (a + ft_strlen((char *)src));
}

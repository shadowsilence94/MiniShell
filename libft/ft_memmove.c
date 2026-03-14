/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:58:51 by hko-ko            #+#    #+#             */
/*   Updated: 2025/08/29 18:58:53 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	const char	*src2;
	char		*dest2;
	size_t		i;

	i = 0;
	src2 = src;
	dest2 = dest;
	if (!dest && !src)
		return (NULL);
	if (dest2 < src2)
	{
		while (i < n)
		{
			dest2[i] = src2[i];
			i++;
		}
	}
	else
	{
		while (n-- > 0)
		{
			dest2[n] = src2[n];
		}
	}
	return (dest2);
}

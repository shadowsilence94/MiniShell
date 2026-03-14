/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 19:21:55 by hko-ko           #+#    #+#             */
/*   Updated: 2025/08/29 19:21:58 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t		i;
	size_t		count;

	i = 0;
	count = 0;
	if (!big || !little)
		return (NULL);
	if (little[0] == '\0')
		return ((char *)&big[i]);
	while (big[i] != '\0' && i < len)
	{
		while (big[i + count] == little[count]
			&& little[count] != '\0' && count + i < len)
		{
			count++;
		}
		if (count == ft_strlen((char *)little))
			return ((char *)&big[i]);
		else
			count = 0;
		i++;
	}
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2025/12/26 12:00:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strlcpy(char *dist, const char *src, size_t len)
{
	size_t	i;

	i = 0;
	if (len > 0)
	{
		while (src[i] != '\0' && i < len - 1)
		{
			dist[i] = src[i];
			i++;
		}
		dist[i] = 0;
	}
	while (src[i])
		i++;
	return (i);
}

// int main() {
//     char source[] = "Hello, World!";
//     char destination[10]; 
//     size_t result;

//     result = ft_strlcpy(destination, source, 5);

//     printf("Source: %s\n", source);
//     printf("Destination: %s\n", destination);
//     printf("Length of source string: %zu\n", result);

//     return 0;
// }

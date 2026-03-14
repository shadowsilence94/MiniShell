/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 13:03:11 by hko-ko           #+#    #+#             */
/*   Updated: 2025/09/05 13:03:14 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <time.h>
#include "libft.h"

void	*ft_calloc(long nmem, long size)
{
	void	*ptr;

	if (nmem == 0 || size == 0)
		return (malloc(1));
	if (nmem < 0 || size < 0)
		return (NULL);
	ptr = malloc(nmem * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, nmem * size);
	return (ptr);
}

// int	main (void)
// {
// 	clock_t tic, toc;
// 	int *array;

// 	tic = clock();
// 	array = malloc(1000000 * sizeof(int));
// 	toc = clock();
// 	printf("malloc: %fs\n", (double) (toc - tic) / CLOCKS_PER_SEC);
// 	free(array);

// 	tic = clock();
// 	array = ft_calloc(1000000 , sizeof(int));
// 	toc = clock();
// 	printf("calloc: %fs\n", (double) (toc - tic) / CLOCKS_PER_SEC);
// 	free(array);
// 	// int size = 0;
// 	// int *junk = 0;
// 	// srand( time(0) );

// 	// for (int i = 0; i < 1000; i++)
// 	// {
// 	// 	size = rand() % 16000;
// 	// 	junk = malloc( size * sizeof(int));
// 	// 	for (int j = 0; j < size; j++)
// 	// 	{
// 	// 		junk[j] = rand();
// 	// 	}
// 	// 	free(junk);
// 	// }
// }

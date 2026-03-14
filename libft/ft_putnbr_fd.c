/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 19:16:08 by hko-ko            #+#    #+#             */
/*   Updated: 2025/08/29 19:16:09 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	numlen(int n)
{
	int	i;

	i = 0;
	while (n > 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

static int	muitply(int n)
{
	int	i;
	int	z;
	int	num;

	z = 1;
	i = 0;
	num = numlen(n) - 1;
	while (i < num)
	{
		z *= 10;
		i++;
	}
	return (z);
}

void	ft_putnbr_fd(int n, int fd)
{
	int		box;
	char	o;

	o = 0;
	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (n < 0)
	{
		write(fd, "-", 1);
		n = -n;
	}
	box = muitply(n);
	while (n > 0 || box >= 1)
	{
		o = (n / box) + 48;
		n %= box;
		box /= 10;
		write(fd, &o, 1);
	}
}

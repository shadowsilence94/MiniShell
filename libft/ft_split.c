/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#             */
/*   Updated: 2025/12/26 12:00:00 by hko-ko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_word_count(char const *s, char c)
{
	size_t	count;
	int		is_word;

	count = 0;
	is_word = 0;
	while (*s)
	{
		if (*s == c)
			is_word = 0;
		else if (is_word == 0)
		{
			is_word = 1;
			count++;
		}
		s++;
	}
	return (count);
}

static void	*ft_free_splitstr(char **splitstr, size_t end)
{
	size_t	i;

	i = 0;
	while (i < end)
	{
		free(splitstr[i]);
		i++;
	}
	free(splitstr);
	return (NULL);
}

static size_t	ft_get_word_len(char const *s, char c)
{
	size_t	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

static char	**ft_add_word(char **splitstr, char const *s, char c)
{
	size_t	word_len;
	size_t	j;

	j = 0;
	while (*s)
	{
		if (*s != c)
		{
			word_len = ft_get_word_len(s, c);
			splitstr[j] = ft_substr(s, 0, word_len);
			if (!splitstr[j])
				return (ft_free_splitstr(splitstr, j));
			s += word_len;
			j++;
		}
		else
			s++;
	}
	return (splitstr);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	size_t	total_words;

	if (!s)
		return (NULL);
	total_words = ft_word_count(s, c);
	res = malloc(sizeof(char *) * (total_words + 1));
	if (!res)
		return (NULL);
	res = ft_add_word(res, s, c);
	if (!res)
		return (NULL);
	res[total_words] = NULL;
	return (res);
}

// int	main(void)
// {
// 	char **res = ft_split("   lorem   ipsum dolor     
// sit amet, consectetur   adipiscing elit. Sed non risus. Suspendisse  ", ' ');

// 	int i = 0;
// 	while (res[i++])
// 		printf("%s\n", res[i]);
// }

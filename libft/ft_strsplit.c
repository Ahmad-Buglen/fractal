/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 21:16:58 by dphyliss          #+#    #+#             */
/*   Updated: 2019/04/27 21:20:26 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(char const *str, char c)
{
	int		count;
	int		i;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] == c)
			i++;
		if (str[i] != c && str[i] != '\0')
			count++;
		while (str[i] != c && str[i] != '\0')
			i++;
	}
	return (count);
}

static int	ft_strc(char const *str, char c, int **i)
{
	int		count;
	int		b;

	b = **i;
	count = 0;
	if (str[b])
	{
		while (str[b] != c && str[b] != '\0')
		{
			b++;
			count++;
		}
	}
	return (count);
}

static char	*ft_word(const char *str, char c, int *i)
{
	char	*s;
	int		k;
	int		z;

	z = ft_strc(str, c, &i);
	if (!(s = (char *)malloc(sizeof(char) * z + 1)))
		return (NULL);
	k = 0;
	while (str[*i] != c && str[*i])
	{
		s[k] = str[*i];
		k++;
		*i += 1;
	}
	s[k] = '\0';
	while (str[*i] == c && str[*i])
		*i += 1;
	return (s);
}

static void	*ft_freeall(char **s, int wrd)
{
	while (wrd >= 0)
	{
		free(s[wrd]);
		wrd--;
	}
	free(s);
	return (NULL);
}

char		**ft_strsplit(const char *str, char c)
{
	int		i;
	int		j;
	int		wrd;
	char	**s;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	wrd = ft_count_words(str, c) + 1;
	if (!(s = (char **)malloc(sizeof(char *) * wrd)))
		return (NULL);
	while (str[i] == c && str[i])
		i++;
	while (j < wrd && str[i])
	{
		if (!(s[j] = ft_word(str, c, &i)))
			ft_freeall(s, j - 1);
		j++;
	}
	s[j] = NULL;
	return (s);
}

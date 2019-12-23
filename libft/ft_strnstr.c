/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/19 20:22:29 by dphyliss          #+#    #+#             */
/*   Updated: 2019/05/04 12:07:32 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	char	*ft_s1(const char *hs, const char *nl, size_t len)
{
	size_t		i;
	size_t		j;

	i = 0;
	while ((hs[i]) && (i < len))
	{
		if (hs[i] == nl[0])
		{
			j = 1;
			while ((hs[++i] == nl[j]) && hs[i] \
					&& nl[j] && (i < len))
				j++;
			if (j == ft_strlen(nl))
				return ((char *)hs + i - j);
			else
				i -= j;
		}
		i++;
	}
	return (NULL);
}

char			*ft_strnstr(const char *haystack, const char *needle,
					size_t len)
{
	if (needle[0] == '\0')
		return ((char *)haystack);
	else
		return (ft_s1(haystack, needle, len));
}

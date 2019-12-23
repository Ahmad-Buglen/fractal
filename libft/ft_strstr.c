/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 15:05:45 by dphyliss          #+#    #+#             */
/*   Updated: 2019/05/04 12:18:58 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char			*ft_strstr(const char *haystack, const char *needle)
{
	long int	i;
	long int	j;

	if (needle[0] == '\0')
		return ((char *)haystack);
	else
	{
		i = -1;
		while (haystack[++i] != '\0')
			if (haystack[i] == needle[0])
			{
				j = 1;
				while ((haystack[++i] == needle[j]) && (haystack[i] != '\0') \
						&& (needle[j] != '\0'))
					j++;
				if ((size_t)j == ft_strlen(needle))
					return ((char *)haystack + i - j);
				else
					i -= j;
			}
		return (NULL);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/22 15:20:14 by dphyliss          #+#    #+#             */
/*   Updated: 2019/05/14 14:35:07 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	char	*str;
	char	*buf;

	if (s1 && s2)
	{
		i = ft_strlen(s1) + ft_strlen(s2);
		str = (char *)malloc(sizeof(char) * i + 1);
		if (str == NULL)
			return (NULL);
		buf = str;
		while (*s1)
			*str++ = *s1++;
		while (*s2)
			*str++ = *s2++;
		*str = '\0';
		return (buf);
	}
	return (NULL);
}

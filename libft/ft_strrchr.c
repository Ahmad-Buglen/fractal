/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/19 20:21:45 by dphyliss          #+#    #+#             */
/*   Updated: 2019/04/28 14:55:13 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;
	int j;

	if ('\0' == (char)c)
	{
		i = 0;
		while (s[i] != '\0')
			i++;
		return ((char *)s + i);
	}
	i = -1;
	j = -1;
	while (s[++i] != '\0')
		if (s[i] == (char)c)
			j = i;
	if (j >= 0)
		return ((char *)s + j);
	else
		return (NULL);
}

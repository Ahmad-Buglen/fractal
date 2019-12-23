/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/22 14:44:59 by dphyliss          #+#    #+#             */
/*   Updated: 2019/04/28 15:38:00 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strsub(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*sub;

	if (s)
	{
		sub = (char *)malloc(sizeof(char) * len + 1);
		if (sub == NULL)
			return (NULL);
		if (len == 0)
			return (sub);
		i = 0;
		j = start;
		while ((i < len) && (s[i] != '\0'))
		{
			sub[i] = s[j];
			i++;
			j++;
		}
		sub[i] = '\0';
		return (sub);
	}
	return (NULL);
}

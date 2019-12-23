/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/15 15:44:16 by dphyliss          #+#    #+#             */
/*   Updated: 2019/04/28 16:59:31 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void				*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t			i;
	char			*str1;
	char			*str2;

	i = 0;
	str1 = (char *)dst;
	str2 = (char *)src;
	if (str2 > str1)
	{
		while (i < len)
		{
			str1[i] = str2[i];
			++i;
		}
	}
	else if (str2 < str1)
	{
		while (len-- != 0)
			str1[len] = str2[len];
	}
	return (dst);
}

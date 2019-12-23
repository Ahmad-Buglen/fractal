/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 13:32:47 by dphyliss          #+#    #+#             */
/*   Updated: 2019/05/03 20:28:53 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	if (s1 == s2)
		return (0);
	else if (n > 0)
	{
		i = 0;
		while (i < n)
		{
			if (((unsigned const char *)s1)[i] != \
					((unsigned const char *)s2)[i])
			{
				return (((unsigned const char *)s1)[i] - \
						((unsigned const char *)s2)[i]);
			}
			i++;
		}
	}
	return (0);
}

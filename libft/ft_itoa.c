/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 15:06:05 by dphyliss          #+#    #+#             */
/*   Updated: 2019/04/29 19:49:18 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int			ft_count(int c)
{
	int				q;

	q = 1;
	while (c / 10)
	{
		c = c / 10;
		q++;
	}
	return (q);
}

char				*ft_itoa(int n)
{
	int				len;
	unsigned int	q;
	char			*res;

	len = ft_count(n);
	len = (n < 0 ? len + 1 : len);
	q = (n < 0 ? -n : n);
	if (!(res = (char *)malloc(sizeof(char) * len + 1)))
		return (NULL);
	res[len--] = '\0';
	while (len >= 0)
	{
		res[len--] = (q % 10) + '0';
		q /= 10;
	}
	if (res[0] == '0' && res[1] != '\0')
		res[0] = '-';
	return (res);
}

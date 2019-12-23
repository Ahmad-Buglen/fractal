/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 17:14:19 by dphyliss          #+#    #+#             */
/*   Updated: 2019/05/02 11:19:26 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strnew(size_t size)
{
	size_t	i;
	char	*point;

	if (18446744073709551615ULL == size)
		return (NULL);
	point = (char *)malloc(sizeof(char) * size + 1);
	if (point == NULL)
		return (NULL);
	else
	{
		i = 0;
		while (i <= size)
		{
			point[i] = '\0';
			i++;
		}
		return (point);
	}
}

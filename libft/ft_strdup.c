/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 14:55:48 by dphyliss          #+#    #+#             */
/*   Updated: 2019/04/28 14:18:23 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strdup(const char *s1)
{
	size_t	i;
	size_t	j;
	char	*duplicate;

	i = 0;
	while (s1[i])
		i++;
	duplicate = (char *)malloc(sizeof(char) * i + 1);
	if (duplicate == NULL)
		return (NULL);
	j = 0;
	while (j <= i)
	{
		duplicate[j] = s1[j];
		j++;
	}
	return (duplicate);
}

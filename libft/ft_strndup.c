/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 15:59:36 by dphyliss          #+#    #+#             */
/*   Updated: 2019/05/14 16:10:09 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strndup(const char *s1, size_t n)
{
	size_t	i;
	char	*duplicate;

	duplicate = (char *)malloc(sizeof(char) * n + 1);
	if (duplicate == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		duplicate[i] = s1[i];
		++i;
	}
	duplicate[n] = '\0';
	return (duplicate);
}

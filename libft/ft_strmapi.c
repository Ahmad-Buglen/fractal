/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 18:29:35 by dphyliss          #+#    #+#             */
/*   Updated: 2019/04/28 15:25:38 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	i;
	size_t	length;
	char	*point;

	if (s && (f != NULL))
	{
		length = ft_strlen(s);
		point = (char *)malloc(sizeof(char) * length + 1);
		if (point == NULL)
			return (NULL);
		else
		{
			i = 0;
			while (s[i] != '\0')
			{
				point[i] = f(i, s[i]);
				++i;
			}
			point[i] = '\0';
			return (point);
		}
	}
	return (NULL);
}

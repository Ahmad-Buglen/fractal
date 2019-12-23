/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 08:14:12 by dphyliss          #+#    #+#             */
/*   Updated: 2019/04/29 16:51:59 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_stri(char const *c)
{
	int		b;

	b = 0;
	while (c[b] == ' ' || c[b] == '\n' || c[b] == '\t')
		b++;
	return (b);
}

char		*ft_strtrim(char const *s)
{
	char	*str;
	int		i;
	int		l;
	int		a;

	a = 0;
	str = "";
	if (!s)
		return ((char *)s);
	i = ft_stri(s);
	l = ft_strlen(s);
	while (s[l - 1] == ' ' || s[l - 1] == '\n' || s[l - 1] == '\t')
		l--;
	if (l == 0)
		return (ft_strnew(0));
	if (!(str = (char *)malloc(sizeof(char) * (l + 1 - i))))
		return (NULL);
	while (i != l)
		str[a++] = s[i++];
	str[a] = '\0';
	return (str);
}

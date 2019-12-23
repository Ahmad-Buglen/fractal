/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 14:31:04 by dphyliss          #+#    #+#             */
/*   Updated: 2019/04/25 14:50:25 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_putnbr_fd(int n, int fd)
{
	char	*s;
	int		sign;
	char	mas[12];

	s = &mas[11];
	if (n == 0)
		ft_putstr_fd("0", fd);
	else if (n == -2147483648)
		ft_putstr_fd("-2147483648", fd);
	else
	{
		*s = '\0';
		sign = (n < 0) ? 1 : 0;
		n = (n < 0) ? -n : n;
		while (n)
		{
			*(--s) = ('0' + (n % 10));
			n /= 10;
		}
		(sign == 1) ? *(--s) = '-' : 0;
		ft_putstr_fd(s, fd);
	}
}

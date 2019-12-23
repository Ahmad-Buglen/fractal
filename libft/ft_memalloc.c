/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 16:47:11 by dphyliss          #+#    #+#             */
/*   Updated: 2019/10/29 18:29:43 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		*ft_memalloc(size_t size)
{
	void	*point;

	if ((0 == size) ||
			(!(point = (void *)malloc(size))))
		return (NULL);
	ft_bzero(point, size);
	return ((void *)point);
}

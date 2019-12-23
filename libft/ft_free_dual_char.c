/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_dual_char.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <admin@dphyliss.42>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 16:25:26 by dphyliss          #+#    #+#             */
/*   Updated: 2019/12/09 18:41:50 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_dual_char(char ***str)
{
	char **buf;

	buf = *str;
	while (*buf)
	{
		ft_strdel(&(*buf));
		++buf;
	}
	free(*str);
	*str = NULL;
}

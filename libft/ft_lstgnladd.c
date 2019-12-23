/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstgnladd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <dphyliss@admin.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 16:57:55 by dphyliss          #+#    #+#             */
/*   Updated: 2019/12/09 18:42:40 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstgnladd(t_list_gnl **alst, t_list_gnl *new)
{
	if (alst && new)
	{
		new->next = *alst;
		*alst = new;
	}
}

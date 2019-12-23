/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 15:37:35 by dphyliss          #+#    #+#             */
/*   Updated: 2019/05/03 20:07:31 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	list_free(t_list *lst)
{
	t_list	*buf;

	while (lst)
	{
		buf = lst;
		lst = lst->next;
		free(buf);
	}
	return (1);
}

t_list		*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list	*new;
	t_list	*list;
	t_list	*buf;

	if (!lst || !f)
		return (NULL);
	list = f(lst);
	new = list;
	while (lst)
	{
		buf = (t_list *)malloc(sizeof(t_list));
		if (!buf && !(f(lst)) && list_free(new))
			return (NULL);
		else
			buf = f(lst);
		lst = lst->next;
		list->next = buf;
		list = list->next;
	}
	list->next = NULL;
	new = new->next;
	return (new);
}

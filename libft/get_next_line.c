/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 14:00:15 by dphyliss          #+#    #+#             */
/*   Updated: 2019/12/09 18:44:13 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_list_gnl		*ft_listnew(int fd)
{
	t_list_gnl			*list;

	if (!(list = (t_list_gnl *)malloc(sizeof(t_list_gnl))))
		return (NULL);
	list->fd = fd;
	list->str = ft_strnew(0);
	list->next = NULL;
	return (list);
}

static void				set_str(t_list_gnl **list, int fd, char **str)
{
	t_list_gnl			*node;
	t_list_gnl			*list_cpy;

	if (!(*list))
		*list = ft_listnew(fd);
	list_cpy = *list;
	while (list_cpy)
	{
		if (list_cpy->fd == fd)
		{
			*str = list_cpy->str;
			return ;
		}
		list_cpy = list_cpy->next;
	}
	node = ft_listnew(fd);
	ft_lstgnladd(list, node);
	*str = (*list)->str;
}

static void				reset_str(t_list_gnl **list, int fd, char **tmp)
{
	t_list_gnl			*list_cpy;

	list_cpy = *list;
	while (list_cpy)
	{
		if (list_cpy->fd == fd)
		{
			if (*tmp)
			{
				list_cpy->str = ft_strdup(*tmp + 1);
				**tmp = '\0';
			}
			else
				list_cpy->str = "\0";
			return ;
		}
		list_cpy = list_cpy->next;
	}
}

static int				is_end_file(char *str, char **line)
{
	if (str[0] == '\0' || !str)
	{
		*line = "";
		return (1);
	}
	return (0);
}

int						get_next_line(const int fd, char **line)
{
	int					ret;
	char				buf[BUFF_SIZE + 1];
	static t_list_gnl	*list;
	char				*str;
	char				*tmp;

	if ((read(fd, buf, 0)) == -1 || !line)
		return (-1);
	str = NULL;
	set_str(&list, fd, &str);
	while (!ft_strchr(str, '\n') && (ret = read(fd, buf, BUFF_SIZE)))
	{
		buf[ret] = '\0';
		tmp = str;
		str = ft_strjoin(tmp, buf);
		ft_strdel(&tmp);
	}
	if (is_end_file(str, line))
		return (0);
	*line = str;
	tmp = ft_strchr(str, '\n');
	reset_str(&list, fd, &tmp);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <admin@21-school.ru>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 20:45:56 by dphyliss          #+#    #+#             */
/*   Updated: 2019/11/30 20:51:58 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	start_pos(t_screen *const screen)
{
	screen->map.scale_x = 30;
	screen->map.scale_y = -30;
	screen->map.pos_x = 400;
	screen->map.pos_y = 400;
	screen->map.alpha = 0;
	screen->map.beta = 0;
	screen->map.gamma = 0;
	screen->map.alt = 1;
}

void	iso_abg(const int keycode, t_screen *const screen)
{
	if (keycode == 18)
		screen->map.alpha -= 0.05;
	else if (keycode == 19)
		screen->map.alpha += 0.05;
	else if (keycode == 20)
		screen->map.beta -= 0.05;
	else if (keycode == 21)
		screen->map.beta += 0.05;
	else if (keycode == 23)
		screen->map.gamma -= 0.05;
	else if (keycode == 22)
		screen->map.gamma += 0.05;
	else if (keycode == 34)
		screen->map.info = (screen->map.info) ? 0 : 1;
	else if (keycode == 49)
		start_pos(screen);
}

int		key_hook(const	int keycode, t_screen *const screen)
{
	if (keycode == 53)
		exit(0);
	iso_abg(keycode, screen);
	if (keycode == 124)
		screen->map.pos_x += 30;
	else if (keycode == 125)
		screen->map.pos_y += 30;
	else if (keycode == 123)
		screen->map.pos_x -= 30;
	else if (keycode == 126)
		screen->map.pos_y -= 30;
	else if (keycode == 13)
	{
		screen->map.alt += 1;
		apply_height(screen, 1);
	}
	else if (keycode == 1)
	{
		screen->map.alt -= 1;
		apply_height(screen, -1);
	}
	print_screen(screen);
	return (1);
}

void	init(t_screen *const screen)
{
	if (!(screen->mlx_ptr = mlx_init()))
		ft_exit("Error: mlx failed.\n");
	if (!(screen->win_ptr = mlx_new_window(screen->mlx_ptr,
					WIN_X, WIN_Y, "asdf")))
		ft_exit("Error: mlx_new_window failed.\n");
	screen->map.scale_x = 30;
	screen->map.scale_y = -30;
	screen->map.pos_x = 400;
	screen->map.pos_y = 400;
	screen->map.alpha = 0;
	screen->map.beta = 0;
	screen->map.gamma = 0;
	screen->map.info = 0;
	screen->map.alt = 1;
	screen->map.color = 0x000000;
	if (!(screen->image.ptr = mlx_new_image(screen->mlx_ptr, WIN_X, WIN_Y)))
		ft_exit("Error: mls_new_image failed.\n");
	apply_proj(screen);
	if (!(screen->image.data = (int *)mlx_get_data_addr(screen->image.ptr,
		&screen->image.bpp, &screen->image.size_line, &screen->image.endian)))
		ft_exit("Error: mls_new_image failed.\n");
}

int		main(int ac, char **av)
{
	t_screen	*screen;
	int			fd;

	if (2 != ac)
		ft_exit("1");
	if (!(screen = (t_screen *)malloc(sizeof(t_screen))))
		ft_exit("Error: the memory hasn't been allocated.\n");
	init(screen);
	if (((fd = open(av[1], O_RDONLY)) == -1))
		ft_exit("Error: Map is invalid.\n");
	if (!(screen->map.data = (t_point **)ft_memalloc(sizeof(t_point *) * 1000)))
		ft_exit("Error: the memory hasn't been allocated.\n");
	read_file(screen, fd);
	close(fd);
	recalc_scale(screen);
	print_screen(screen);
	mlx_hook(screen->win_ptr, 2, 0, key_hook, screen);
	mlx_hook(screen->win_ptr, 4, 0, mouse_hook, screen);
	mlx_loop(screen->mlx_ptr);
	return (1);
}

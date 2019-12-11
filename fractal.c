/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <admin@21-school.ru>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 20:45:56 by dphyliss          #+#    #+#             */
/*   Updated: 2019/12/11 15:20:28 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdio.h>
# include <math.h>
# include "mlx.h"
# include "libft/libft.h"

# define WIN_X 1000
# define WIN_Y 1000

typedef	struct	s_point
{
	int			x;
	int			y;
}				t_point;

typedef	struct	s_image
{
	int			*ptr;
	int			*data;
	int			bpp;
	int			size_line;
	int			endian;
}				t_image;

typedef	struct	s_screen
{
	void		*mlx_ptr;
	void		*win_ptr;
	int			height;
	int			width;
	t_image		image;
}				t_screen;

t_complex init_complex(double re, double im)
{
	t_complex complex;

	complex.re = re;
	complex.im = im;
	return (complex);
}

void	init(t_screen *const screen)
{
	if (!(screen->mlx_ptr = mlx_init()))
		ft_exit("Error: mlx failed.\n");
	if (!(screen->win_ptr = mlx_new_window(screen->mlx_ptr,
					WIN_X, WIN_Y, "asdf")))
		ft_exit("Error: mlx_new_window failed.\n");
	if (!(screen->image.ptr = mlx_new_image(screen->mlx_ptr, WIN_X, WIN_Y)))
		ft_exit("Error: mls_new_image failed.\n");
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

	min.re = -2.0;
	max.re = 2.0;
	min.im = -2.0;
	max.im = min.im + (max.re - min.re) * HEIGHT / WIDTH;

	factor.re = (max.re - min.re) / (WIDTH - 1);
	factor.im = (max.im - min.im) / (HEIGHT - 1);

	max_iteration = 50;
	y = 0;
	while (y < WIN_Y)
	{
		c.im = max.im - y * factor.im;
		x = 0;
		while (x < WIN_X)
		{
			c.re = min.re + x * factor.re;
			z = init_complex(c.re, c.im);
			iteration = 0;
			while (pow(z.re, 2.0) + pow(z.im, 2.0) <= 4
					&& iteration < max_iteration)
			{
				z = init_complex(
						pow(z.re, 2.0) - pow(z.im, 2.0) + c.re,
						2.0 * z.re * z.im + c.im);
				iteration++;
			}
			t = (double)iteration / (double)max_iteration;

			red = (int)(9 * (1 - t) * pow(t, 3) * 255);
			green = (int)(15 * pow((1 - t), 2) * pow(t, 2) * 255);
			blue = (int)(8.5 * pow((1 - t), 3) * t * 255);
			//Установка цвета точки
			x++;
		}
		y++;
	}

	mlx_hook(screen->win_ptr, 2, 0, key_hook, screen);
	mlx_hook(screen->win_ptr, 4, 0, mouse_hook, screen);
	mlx_loop(screen->mlx_ptr);
	return (1);
}

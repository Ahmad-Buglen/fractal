/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <admin@21-school.ru>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 20:45:56 by dphyliss          #+#    #+#             */
/*   Updated: 2019/12/11 19:08:57 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <math.h>
# include "mlx.h"
//# include "libft/libft.h"

# define WIN_X 1300
# define WIN_Y 1300

typedef	struct	s_point
{
	int			x;
	int			y;
}				t_point;

typedef	struct	s_compl
{
	double			im;
	double			re;
}				t_compl;

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

void		ft_putstr_fd(char const *s, int fd)
{
	size_t	i;

	if (s && (fd >= 0))
	{
		i = 0;
		while (s[i] != '\0')
		{
			write(fd, &s[i], 1);
			++i;
		}
	}
}

void    ft_exit(const char *const str)
{
	ft_putstr_fd(str, 2);
	exit(0);
}



t_compl init_compl(double re, double im)
{
	t_compl compl;

	compl.re = re;
	compl.im = im;
	return (compl);
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

int     key_hook(const  int keycode, t_screen *const screen)
{
	if (keycode == 53)
		exit(0);
	return (1);
}

int		main(int ac, char **av)
{
	t_screen	*screen;
	int			fd;
	int			x;
	int			y;
	int			red;
	int			green;
	int			blue;
	int			max_iteration;
	int			iteration;
	double		t;
	t_compl min;
	t_compl max;
	t_compl factor;
	t_compl c;
	t_compl z;

//	if (2 != ac)
//		ft_exit("1");
	if (!(screen = (t_screen *)malloc(sizeof(t_screen))))
		ft_exit("Error: the memory hasn't been allocated.\n");
	init(screen);
	
	min.re = -2.0;
	max.re = 2.0;
	min.im = -2.0;
	max.im = min.im + (max.re - min.re) * WIN_Y / WIN_X;

	factor.re = (max.re - min.re) / (WIN_X - 1);
	factor.im = (max.im - min.im) / (WIN_Y - 1);

	max_iteration = 113;
	y = 0;
	while (y < WIN_Y)
	{
		c.im = max.im - y * factor.im;
		x = 0;
		while (x < WIN_X)
		{
			c.re = min.re + x * factor.re;
			z = init_compl(c.re, c.im);
			iteration = 0;
			while (pow(z.re, 2.0) + pow(z.im, 2.0) <= 4
					&& iteration < max_iteration)
			{
				z = init_compl(
						pow(z.re, 2.0) - pow(z.im, 2.0) + c.re,
						2.0 * z.re * z.im + c.im);
				iteration++;
			}
			t = (double)iteration / (double)max_iteration;

			red = (int)(9 * (1 - t) * pow(t, 3) * 255);
			green = (int)(15 * pow((1 - t), 2) * pow(t, 2) * 255);
			blue = (int)(8.5 * pow((1 - t), 3) * t * 255);
			//Установка цвета точки
			screen->image.data[WIN_X * y + x] = red * green * blue;
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(screen->mlx_ptr, screen->win_ptr,
						screen->image.ptr, 0, 0);
	mlx_hook(screen->win_ptr, 2, 0, key_hook, screen);
//	mlx_hook(screen->win_ptr, 4, 0, mouse_hook, screen);
	mlx_loop(screen->mlx_ptr);
	return (1);
}

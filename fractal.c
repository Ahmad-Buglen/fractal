/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <admin@21-school.ru>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 20:45:56 by dphyliss          #+#    #+#             */
/*   Updated: 2019/12/16 13:49:18 by dphyliss         ###   ########.fr       */
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

typedef struct	s_fractal
{
	int			red;
	int			green;
	int			blue;
	double		t;
	int			max_iteration;
	t_compl		min;
	t_compl		max;
	t_compl		factor;
	t_compl		c;
	t_compl		z;
}				t_fractal;

typedef	struct	s_screen
{
	void		*mlx_ptr;
	void		*win_ptr;
	int			height;
	int			width;
	t_image		image;
	t_fractal	fractal;
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
	screen->fractal.min.re = -2.0;
	screen->fractal.max.re = 2.0;
	screen->fractal.min.im = -2.0;
	screen->fractal.max.im = screen->fractal.min.im +
		(screen->fractal.max.re - screen->fractal.min.re) * WIN_Y / WIN_X;

	screen->fractal.factor.re = (screen->fractal.max.re -
			screen->fractal.min.re) / (WIN_X - 1);
	screen->fractal.factor.im = (screen->fractal.max.im -
			screen->fractal.min.im) / (WIN_Y - 1);
	screen->fractal.max_iteration = 2;
}


void print_fractal(t_screen * const screen)
{
	int			iteration;
	int			x;
	int			y;
	
	y = 0;
	while (y < WIN_Y)
	{
		screen->fractal.c.im = screen->fractal.max.im - y *
			screen->fractal.factor.im;
		x = 0;
		while (x < WIN_X)
		{
			screen->fractal.c.re = screen->fractal.min.re + x *
				screen->fractal.factor.re ;
			screen->fractal.z = init_compl(screen->fractal.c.re,
					screen->fractal.c.im);
			iteration = 0;
			while (pow(screen->fractal.z.re, 2.0) +
					pow(screen->fractal.z.im, 2.0) <= 4
					&& iteration < screen->fractal.max_iteration)
			{
				screen->fractal.z = init_compl(
						pow(screen->fractal.z.re, 2.0) -
						pow(screen->fractal.z.im, 2.0) + screen->fractal.c.re,
						2.0 * screen->fractal.z.re * screen->fractal.z.im +
						screen->fractal.c.im);
				iteration++;
			}
			screen->fractal.t = (double)iteration /
				(double)screen->fractal.max_iteration;

			screen->fractal.red = (int)(9 * (1 - screen->fractal.t) *
					pow(screen->fractal.t, 3) * 255);
			screen->fractal.green = (int)(15 * pow((1 - screen->fractal.t), 2) *
					pow(screen->fractal.t, 2) * 255);
			screen->fractal.blue = (int)(8.5 * pow((1 - screen->fractal.t), 3) *
					screen->fractal.t * 255);
			//Установка цвета точки
			screen->image.data[WIN_X * y + x] = screen->fractal.red *
				screen->fractal.green * screen->fractal.blue;
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(screen->mlx_ptr, screen->win_ptr,
						screen->image.ptr, 0, 0);
}

int     key_hook(const  int keycode, t_screen * const screen)
{
	if (keycode == 53)
		exit(0);
	else
	{
		screen->fractal.max_iteration += 1;
		print_fractal(screen);
	}
	return (1);
}

int		main(int ac, char **av)
{
	t_screen	*screen;
	int			fd;

//	if (2 != ac)
//		ft_exit("1");
	if (!(screen = (t_screen *)malloc(sizeof(t_screen))))
		ft_exit("Error: the memory hasn't been allocated.\n");
	init(screen);
	print_fractal(screen);
	

	
	mlx_hook(screen->win_ptr, 2, 0, key_hook, screen);
//	mlx_hook(screen->win_ptr, 4, 0, mouse_hook, screen);
	mlx_loop(screen->mlx_ptr);
	return (1);
}

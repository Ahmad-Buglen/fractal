/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractal.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <admin@21-school.ru>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 19:10:21 by dphyliss          #+#    #+#             */
/*   Updated: 2020/01/08 19:10:25 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractal.h"

void	init(t_screen *const s, const int init)
{
	if (1 == init)
	{
		if ((!(s->mlx_ptr = mlx_init())) ||
		(!(s->win_ptr = mlx_new_window(s->mlx_ptr, WIN_X, WIN_Y, "fractal"))))
			ft_exit("Error: mlx init or new_window failed.");
		if (!(s->image.ptr = mlx_new_image(s->mlx_ptr, WIN_X, WIN_Y)))
			ft_exit("Error: mlx_new_image failed.\n");
		if (!(s->image.data = (int *)mlx_get_data_addr(s->image.ptr,
			&s->image.bpp, &s->image.size_line, &s->image.endian)))
			ft_exit("Error: mlx_new_image failed.\n");
		s->opcl.kernel = 0;
		s->opcl.total_s = WIN_X * WIN_Y;
		s->info = 0;
	}
	s->fractal.min = init_compl(-2.0, -2.0);
	s->fractal.max = init_compl(2.0, (s->fractal.min.im +
				(2.0 - s->fractal.min.re) * WIN_Y / WIN_X));
	s->fractal.max_iteration = 50;
	if (2 == s->fractal.number)
	{
		s->fractal.k = init_compl(-0.4, 0.6);
		s->fractal.fixed = 0;
		s->fractal.julia = 1;
	}
	s->fractal.color = 2;
}

void	set_fractal(t_screen *const s, const int number)
{
	if (1 == number)
		s->fractal.function = "mandelbrot";
	else if (2 == number)
		s->fractal.function = "julia";
	else if (3 == number)
		s->fractal.function = "burning_ship";
	else if (4 == number)
		s->fractal.function = "mandelbar";
	else if (5 == number)
		s->fractal.function = "celtic_mandelbrot";
	else if (6 == number)
		s->fractal.function = "celtic_mandelbar";
	else if (7 == number)
		s->fractal.function = "celtic_perpendicular";
	else if (8 == number)
		s->fractal.function = "perpendicular_mandelbrot";
	else if (9 == number)
		s->fractal.function = "perpendicular_burning_ship";
	else if (10 == number)
		s->fractal.function = "perpendicular_buffalo";
	s->fractal.number = number;
}

void	ft_usage(int ac)
{
	int i;

	i = ac;
	ft_putendl("Usage:");
	ft_putendl("1:\tMandelbrot");
	ft_putendl("2:\tJulia");
	ft_putendl("3:\tBurning ship");
	ft_putendl("4:\tMandelbar");
	ft_putendl("5:\tCeltic Mandelbrot");
	ft_putendl("6:\tCeltic Mandelbar");
	ft_putendl("7:\tCeltic perpendicular");
	ft_putendl("8:\tPerpendicular Mandelbrot");
	ft_putendl("9:\tPerpendicular burning ship");
	ft_putendl("10:\tPerpendicular buffalo");
	exit(0);
	return ;
}

int		main(int ac, char **av)
{
	t_screen	*s;

	if ((2 != ac) || ((ft_atoi(av[1]) < 1) || (ft_atoi(av[1]) > 10)))
		ft_usage(ac);
	if (!(s = (t_screen *)malloc(sizeof(t_screen))))
		ft_exit("Error: the memory hasn't been allocated.\n");
	set_fractal(s, ft_atoi(av[1]));
	init(s, 1);
	init_cl(s);
	draw(s);
	mlx_hook(s->win_ptr, 17, 0, ft_close, s);
	mlx_hook(s->win_ptr, 2, 0, key_hook, s);
	mlx_hook(s->win_ptr, 6, 0, julia_motion, s);
	mlx_hook(s->win_ptr, 4, 0, mouse_hook, s);
	mlx_loop(s->mlx_ptr);
	return (1);
}

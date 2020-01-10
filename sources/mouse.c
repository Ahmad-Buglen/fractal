/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <admin@21-school.ru>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 19:09:20 by dphyliss          #+#    #+#             */
/*   Updated: 2020/01/08 19:09:22 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractal.h"

void	ft_exit(const char *const str)
{
	ft_putendl(str);
	exit(0);
}

t_compl	init_compl(const double re, const double im)
{
	t_compl compl;

	compl.re = re;
	compl.im = im;
	return (compl);
}

void	free_cl(t_screen *const s)
{
	clReleaseContext(s->opcl.context);
	clReleaseCommandQueue(s->opcl.queue);
	clReleaseProgram(s->opcl.program);
	clReleaseKernel(s->opcl.kernel);
	clReleaseMemObject(s->opcl.buf);
}

int		mouse_hook(const int keycode, const int x, const int y,
					t_screen *const s)
{
	t_compl		mouse;
	double		interpolation;
	double		zoom;
	t_fractal	*f;

	f = &s->fractal;
	if (keycode == 4 || keycode == 5)
	{
		mouse = init_compl(
			(double)x / (WIN_X / (f->max.re - f->min.re)) + f->min.re,
			(double)y / (WIN_Y / (f->max.im - f->min.im)) * -1 + f->max.im);
		if (keycode == 4)
			zoom = 0.80;
		else
			zoom = 1.20;
		interpolation = 1.0 / zoom;
		f->min.re = mouse.re + ((f->min.re - mouse.re) * interpolation);
		f->min.im = mouse.im + ((f->min.im - mouse.im) * interpolation);
		f->max.re = mouse.re + ((f->max.re - mouse.re) * interpolation);
		f->max.im = mouse.im + ((f->max.im - mouse.im) * interpolation);
		draw(s);
	}
	return (1);
}

int		julia_motion(const int x, const int y, t_screen *const s)
{
	if (s->fractal.julia && !s->fractal.fixed)
	{
		s->fractal.k.re = 4 * ((double)x / WIN_X - 0.5);
		s->fractal.k.im = 4 * ((double)(WIN_Y - y) / WIN_Y - 0.5);
		draw(s);
	}
	return (0);
}

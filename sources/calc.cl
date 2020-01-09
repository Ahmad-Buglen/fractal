#include "includes/cl.h"

int set_colors(unsigned char o, unsigned char red, \
				unsigned char green, unsigned char blue)
{
	int i;
	int tmp;
	int res;
	int j;

	i = 0;
	res = 0;
	tmp = 0;
	j = 0;
	while (i <= 31)
	{
		tmp = (i >= 0 && i <= 7) ? blue : tmp;
		tmp = (i >= 8 && i <= 15) ? green : tmp;
		tmp = (i >= 16 && i <= 23) ? red : tmp;
		tmp = (i >= 24 && i <= 31) ? o : tmp;
		j = 0;
		while (j <= 7)
		{
			if (tmp & (1 << j))
			res |= 1 << i;
			++i;
			++j;
		}
	}
	return (res);
}

int choose_color(int i, int max, char color)
{
	int			red;
	int			blue;
	int			green;
	double		n;

	n = (double)i / (double)max;
	red = (int)(9 * (1 - n) * (n * n * n) * 255);
	green = (int)(15 * ((1 - n) * (1 - n)) * (n * n) * 255);
	blue = (int)(8.5 * ((1 - n) * (1 - n) * (1 - n)) * n * 255);
	if (3 == color)
		return (set_colors(0, red, green, blue));
	else if (0 == color)
		return (set_colors(0, blue, green, red));
	else if (2 == color)
		return (set_colors(0, blue, red, green));
	else if (1 == color)
	 	return (set_colors(0, red, blue, green));
	return (0);
}

double	ft_abs(double x)
{
	return ((x < 0) ? -x : x);
}

__kernel void mandelbrot(__global int *data, t_fractal f, int color)
{
	int			iteration;
	int			x;
	int			y;
	int 		gid;
	double		tmp;

 	gid = get_global_id(0);
    x = gid % WIN_X;
	y = gid / WIN_Y;
	f.c.im = f.max.im - y * f.factor.im;
	f.c.re = f.min.re + x *	f.factor.re;
	f.z.re = f.c.re;
	f.z.im = f.c.im;
	iteration = 0;
	while ((f.z.re * f.z.re) + (f.z.im * f.z.im) <= 4
			&& iteration < f.max_iteration)
	{
		tmp = f.z.im;
		f.z.im = 2.0 * f.z.im * f.z.re + f.c.im;
		f.z.re = (f.z.re * f.z.re) - (tmp * tmp) + f.c.re;
		++iteration;
	}
	if (iteration <  f.max_iteration)
 		data[gid] = choose_color(iteration, f.max_iteration, color);
 	else
        data[gid] = 0;
}


__kernel void julia(__global int *data, t_fractal f, int color)
{
	int			iteration;
	int			x;
	int			y;
	int 		gid;
	double		tmp;

 	gid = get_global_id(0);
    x = gid % WIN_X;
	y = gid / WIN_Y;
	f.c.im = f.max.im - y * f.factor.im;
	f.c.re = f.min.re + x *	f.factor.re;
	f.z.re = f.c.re;
	f.z.im = f.c.im;
	iteration = 0;
	while ((f.z.re * f.z.re) + (f.z.im * f.z.im) <= 4
			&& iteration < f.max_iteration)
	{
		tmp = f.z.im;
		f.z.im = 2.0 * f.z.im * f.z.re + f.k.im;
		f.z.re = (f.z.re * f.z.re) - (tmp * tmp) + f.k.re;
		++iteration;
	}
	if (iteration <  f.max_iteration)
 	    data[gid] = choose_color(iteration, f.max_iteration, color);
 	else
        data[gid] = 0;
}

__kernel void burning_ship(__global int *data, t_fractal f, int color)
{
	int			iteration;
	int			x;
	int			y;
	int 		gid;
	double		tmp;

 	gid = get_global_id(0);
    x = gid % WIN_X;
	y = gid / WIN_Y;
	f.c.im = f.max.im - y * f.factor.im;
	f.c.re = f.min.re + x *	f.factor.re;
	f.z.re = f.c.re;
	f.z.im = f.c.im;
	iteration = 0;
	while ((f.z.re * f.z.re) + (f.z.im * f.z.im) <= 4
			&& iteration < f.max_iteration)
	{
		tmp = f.z.im;
		f.z.im = 2.0 * ft_abs(f.z.re * tmp) + f.c.im;
		f.z.re = (f.z.re * f.z.re) - (tmp * tmp) + f.c.re;
		++iteration;
	}
	if (iteration <  f.max_iteration)
 	    data[gid] = choose_color(iteration, f.max_iteration, color);
 	else
        data[gid] = 0;
}

__kernel void mandelbar(__global int *data, t_fractal f, int color)
{
	int			iteration;
	int			x;
	int			y;
	int 		gid;
	double		tmp;

 	gid = get_global_id(0);
    x = gid % WIN_X;
	y = gid / WIN_Y;
	f.c.im = f.max.im - y * f.factor.im;
	f.c.re = f.min.re + x *	f.factor.re;
	f.z.re = f.c.re;
	f.z.im = f.c.im;
	iteration = 0;
	while ((f.z.re * f.z.re) + (f.z.im * f.z.im) <= 4
			&& iteration < f.max_iteration)
	{
		tmp = f.z.re;
		f.z.re = (f.z.re * f.z.re) - (f.z.im * f.z.im) + f.c.re;
		f.z.im = -2.0 * tmp * f.z.im + f.c.im;
		++iteration;	
	}
	if (iteration <  f.max_iteration)
 	    data[gid] = choose_color(iteration, f.max_iteration, color);
 	else
        data[gid] = 0;
}

__kernel void celtic_mandelbrot(__global int *data, t_fractal f, int color)
{
	int			iteration;
	int			x;
	int			y;
	int 		gid;
	double		tmp;

 	gid = get_global_id(0);
    x = gid % WIN_X;
	y = gid / WIN_Y;
	f.c.im = f.max.im - y * f.factor.im;
	f.c.re = f.min.re + x *	f.factor.re;
	f.z.re = f.c.re;
	f.z.im = f.c.im;
	iteration = 0;
	while ((f.z.re * f.z.re) + (f.z.im * f.z.im) <= 4
			&& iteration < f.max_iteration)
	{
		tmp = f.z.re;
		f.z.re = ft_abs((f.z.re * f.z.re) - (f.z.im * f.z.im)) + f.c.re;
		f.z.im = 2.0 * tmp * f.z.im + f.c.im;
		++iteration;	
	}
	if (iteration <  f.max_iteration)
 	    data[gid] = choose_color(iteration, f.max_iteration, color);
 	else
        data[gid] = 0;
}

__kernel void celtic_mandelbar(__global int *data, t_fractal f, int color)
{
	int			iteration;
	int			x;
	int			y;
	int 		gid;
	double		tmp;

 	gid = get_global_id(0);
    x = gid % WIN_X;
	y = gid / WIN_Y;
	f.c.im = f.max.im - y * f.factor.im;
	f.c.re = f.min.re + x *	f.factor.re;
	f.z.re = f.c.re;
	f.z.im = f.c.im;
	iteration = 0;
	while ((f.z.re * f.z.re) + (f.z.im * f.z.im) <= 4
			&& iteration < f.max_iteration)
	{
		tmp = f.z.re;
		f.z.re = ft_abs((f.z.re * f.z.re) - (f.z.im * f.z.im)) + f.c.re;
		f.z.im = -2.0 * tmp * f.z.im + f.c.im;
		++iteration;	
	}
	if (iteration <  f.max_iteration)
 	    data[gid] = choose_color(iteration, f.max_iteration, color);
 	else
        data[gid] = 0;
}

__kernel void celtic_perpendicular(__global int *data, t_fractal f, int color)
{
	int			iteration;
	int			x;
	int			y;
	int 		gid;
	double		tmp;

 	gid = get_global_id(0);
    x = gid % WIN_X;
	y = gid / WIN_Y;
	f.c.im = f.max.im - y * f.factor.im;
	f.c.re = f.min.re + x *	f.factor.re;
	f.z.re = f.c.re;
	f.z.im = f.c.im;
	iteration = 0;
	while ((f.z.re * f.z.re) + (f.z.im * f.z.im) <= 4
			&& iteration < f.max_iteration)
	{
		tmp = f.z.re;
		f.z.re = ft_abs((f.z.re * f.z.re) - (f.z.im * f.z.im)) + f.c.re;
		f.z.im = -2.0 * ft_abs(tmp) * f.z.im + f.c.im;
		++iteration;	
	}
	if (iteration <  f.max_iteration)
 	    data[gid] = choose_color(iteration, f.max_iteration, color);
 	else
        data[gid] = 0;
}

__kernel void perpendicular_mandelbrot(__global int *data,
											t_fractal f, int color)
{
	int			iteration;
	int			x;
	int			y;
	int 		gid;
	double		tmp;

 	gid = get_global_id(0);
    x = gid % WIN_X;
	y = gid / WIN_Y;
	f.c.im = f.max.im - y * f.factor.im;
	f.c.re = f.min.re + x *	f.factor.re;
	f.z.re = f.c.re;
	f.z.im = f.c.im;
	iteration = 0;
	while ((f.z.re * f.z.re) + (f.z.im * f.z.im) <= 4
			&& iteration < f.max_iteration)
	{
		tmp = f.z.re;
		f.z.re = (f.z.re * f.z.re) - (f.z.im * f.z.im) + f.c.re;
		f.z.im = -2.0 * ft_abs(tmp) * f.z.im + f.c.im;
		++iteration;	
	}
	if (iteration <  f.max_iteration)
 	    data[gid] = choose_color(iteration, f.max_iteration, color);
 	else
        data[gid] = 0;
}

__kernel void perpendicular_burning_ship(__global int *data,
											t_fractal f, int color)
{
	int			iteration;
	int			x;
	int			y;
	int 		gid;
	double		tmp;

 	gid = get_global_id(0);
    x = gid % WIN_X;
	y = gid / WIN_Y;
	f.c.im = f.max.im - y * f.factor.im;
	f.c.re = f.min.re + x *	f.factor.re;
	f.z.re = f.c.re;
	f.z.im = f.c.im;
	iteration = 0;
	while ((f.z.re * f.z.re) + (f.z.im * f.z.im) <= 4
			&& iteration < f.max_iteration)
	{
		tmp = f.z.re;
		f.z.re = (f.z.re * f.z.re) - (f.z.im * f.z.im) + f.c.re;
		f.z.im = -2.0 * tmp * ft_abs(f.z.im) + f.c.im;
		++iteration;	
	}
	if (iteration <  f.max_iteration)
 	    data[gid] = choose_color(iteration, f.max_iteration, color);
 	else
        data[gid] = 0;
}

__kernel void perpendicular_buffalo(__global int *data, t_fractal f, int color)
{
	int			iteration;
	int			x;
	int			y;
	int 		gid;
	double		tmp;

 	gid = get_global_id(0);
    x = gid % WIN_X;
	y = gid / WIN_Y;
	f.c.im = f.max.im - y * f.factor.im;
	f.c.re = f.min.re + x *	f.factor.re;
	f.z.re = f.c.re;
	f.z.im = f.c.im;
	iteration = 0;
	while ((f.z.re * f.z.re) + (f.z.im * f.z.im) <= 4
			&& iteration < f.max_iteration)
	{
		tmp = f.z.re;
		f.z.re = ft_abs((f.z.re * f.z.re) - (f.z.im * f.z.im)) + f.c.re;
		f.z.im = -2.0 * tmp * ft_abs(f.z.im) + f.c.im;
		++iteration;	
	}
	if (iteration <  f.max_iteration)
 	    data[gid] = choose_color(iteration, f.max_iteration, color);
 	else
        data[gid] = 0;
}
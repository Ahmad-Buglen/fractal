#include "cl.h"

int choose_color(int i, int max, int color);
int set_colors(unsigned char o, unsigned char red, \
				unsigned char green, unsigned char blue);
int	ft_abs(int x);

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

int choose_color(int i, int max, int color)
{
	int			red;
	int			blue;
	int			green;
	double		n;

	n = (double)i / (double)max;
	red = (int)(9 * (1 - n) * (n * n * n) * 255);
	green = (int)(15 * ((1 - n) * (1 - n)) * (n * n) * 255);
	blue = (int)(8.5 * ((1 - n) * (1 - n) * (1 - n)) * n * 255);
	if (1 == color)
		return (set_colors(0, red, blue, green));
	else if (0 == color)
		return (set_colors(0, blue, green, red));
	else if (2 == color)
		return (set_colors(0, blue, red, green));
	else if (3 == color)
		return (set_colors(0, red, green, blue));
	return (0);
}

int	ft_abs(int x)
{
	return ((x < 0) ? -x : x);
}

__kernel void mandelbrot(__global int *data, t_fractal f)
{
	int			iteration;
	int			x;
	int			y;
	int 		gid;

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
		f.z.im = 2.0 * f.z.im * f.z.re + f.c.im;
		f.z.re = (f.z.re * f.z.re) - (f.z.im * f.z.im) + f.c.re;
		++iteration;
	}
	if (iteration <  f.max_iteration)
 	    data[gid] = choose_color(iteration, f.max_iteration, 0);
 	else
        data[gid] = 0;
}


__kernel void julia(__global int *data, t_fractal f)
{
	int			iteration;
	int			x;
	int			y;
	int 		gid;

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
		f.z.im = 2.0 * f.z.im * f.z.re + f.k.im;
		f.z.re = (f.z.re * f.z.re) - (f.z.im * f.z.im) + f.k.re;
		++iteration;
	}
	if (iteration <  f.max_iteration)
 	    data[gid] = choose_color(iteration, f.max_iteration, f.color);
 	else
        data[gid] = 0;
}

__kernel void burning_ship(__global int *data, t_fractal f)
{
	int			iteration;
	int			x;
	int			y;
	int 		gid;

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
		f.z.im = -2.0 * ft_abs(f.z.im * f.z.re) + f.c.im;
		f.z.re = (f.z.re * f.z.re) - (f.z.im * f.z.im) + f.c.re;
		++iteration;
	}
	if (iteration <  f.max_iteration)
 	    data[gid] = choose_color(iteration, f.max_iteration, f.color);
 	else
        data[gid] = 0;
}

__kernel void mandelbar(__global int *data, t_fractal f)
{
	int			iteration;
	int			x;
	int			y;
	int 		gid;

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
		f.z.re = (f.z.re * f.z.re) - (f.z.im * f.z.im) + f.c.re;
		f.z.im = -2.0 * f.z.re * f.z.im + f.c.im;
		++iteration;	
	}
	if (iteration <  f.max_iteration)
 	    data[gid] = choose_color(iteration, f.max_iteration, f.color);
 	else
        data[gid] = 0;
}

__kernel void celtic_mandelbrot(__global int *data, t_fractal f)
{
	int			iteration;
	int			x;
	int			y;
	int 		gid;

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
		f.z.re = ft_abs((f.z.re * f.z.re) - (f.z.im * f.z.im)) + f.c.re;
		f.z.im = 2.0 * f.z.re * f.z.im + f.c.im;
		++iteration;	
	}
	if (iteration <  f.max_iteration)
 	    data[gid] = choose_color(iteration, f.max_iteration, f.color);
 	else
        data[gid] = 0;
}

__kernel void celtic_mandelbar(__global int *data, t_fractal f)
{
	int			iteration;
	int			x;
	int			y;
	int 		gid;

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
		f.z.re = ft_abs((f.z.re * f.z.re) - (f.z.im * f.z.im)) + f.c.re;
		f.z.im = -2.0 * f.z.re * f.z.im + f.c.im;
		++iteration;	
	}
	if (iteration <  f.max_iteration)
 	    data[gid] = choose_color(iteration, f.max_iteration, f.color);
 	else
        data[gid] = 0;
}

__kernel void celtic_perpendicular(__global int *data, t_fractal f)
{
	int			iteration;
	int			x;
	int			y;
	int 		gid;

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
		f.z.re = ft_abs((f.z.re * f.z.re) - (f.z.im * f.z.im)) + f.c.re;
		f.z.im = -2.0 * ft_abs(f.z.re) * f.z.im + f.c.im;
		++iteration;	
	}
	if (iteration <  f.max_iteration)
 	    data[gid] = choose_color(iteration, f.max_iteration, f.color);
 	else
        data[gid] = 0;
}

__kernel void perpendicular_mandelbrot(__global int *data, t_fractal f)
{
	int			iteration;
	int			x;
	int			y;
	int 		gid;

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
		f.z.re = (f.z.re * f.z.re) - (f.z.im * f.z.im) + f.c.re;
		f.z.im = -2.0 * ft_abs(f.z.re) * f.z.im + f.c.im;
		++iteration;	
	}
	if (iteration <  f.max_iteration)
 	    data[gid] = choose_color(iteration, f.max_iteration, f.color);
 	else
        data[gid] = 0;
}

__kernel void perpendicular_burning_ship(__global int *data, t_fractal f)
{
	int			iteration;
	int			x;
	int			y;
	int 		gid;

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
		f.z.re = (f.z.re * f.z.re) - (f.z.im * f.z.im) + f.c.re;
		f.z.im = -2.0 * f.z.re * ft_abs(f.z.im) + f.c.im;
		++iteration;	
	}
	if (iteration <  f.max_iteration)
 	    data[gid] = choose_color(iteration, f.max_iteration, f.color);
 	else
        data[gid] = 0;
}

__kernel void perpendicular_buffalo(__global int *data, t_fractal f)
{
	int			iteration;
	int			x;
	int			y;
	int 		gid;

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
		f.z.re = ft_abs((f.z.re * f.z.re) - (f.z.im * f.z.im)) + f.c.re;
		f.z.im = -2.0 * f.z.re * ft_abs(f.z.im) + f.c.im;
		++iteration;	
	}
	if (iteration <  f.max_iteration)
 	    data[gid] = choose_color(iteration, f.max_iteration, f.color);
 	else
        data[gid] = 0;
}

// mandelbrot
// julia
// burning_ship
// mandelbar
// celtic_mandelbrot
// celtic_mandelbar
// celtic_perpendicular
// perpendicular_mandelbrot
// perpendicular_burning_ship
// perpendicular_buffalo

	// double		t;
	// double		red;
	// double		green;
	// double		blue;

		// t = (double)iteration /
	// 	(double)f.max_iteration;
	// red = (int)(9 * (1 - t) * pow(t, 3) * 255);
	// green = (int)(15 * pow((1 - t), 2) * pow(t, 2) * 255);
	// blue = (int)(8.5 * pow((1 - t), 3) * t * 255);
	// data[gid] = red * green * blue;

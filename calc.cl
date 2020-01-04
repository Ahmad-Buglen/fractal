# define WIN_X 1024
# define WIN_Y 1024

#include "cl.h"

int		set_colors(unsigned char o, unsigned char r, \
			unsigned char g, unsigned char b)
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
		tmp = (i >= 0 && i <= 7) ? b : tmp;
		tmp = (i >= 8 && i <= 15) ? g : tmp;
		tmp = (i >= 16 && i <= 23) ? r : tmp;
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

int 		choose_color(int i, int max, int color)
{
	int			red;
	int			blue;
	int			green;
	double		n;

	n = (double)i / (double)max;
	if (color == 1)
	{
		red = (int)(9 * (1 - n) * pow(n, 3) * 255);
		green = (int)(15 * pow((1 - n), 2) * pow(n, 2) * 255);
		blue = (int)(8.5 * pow((1 - n), 3) * n * 255);
		return (set_colors(0, red, blue, green));
	}
	else if (color == 0)
	{
		red = (int)(9 * (1 - n) * pow(n, 3) * 255);
		green = (int)(15 * pow((1 - n), 2) * pow(n, 2) * 255);
		blue = (int)(8.5 * pow((1 - n), 3) * n * 255);
		return (set_colors(0, blue, green, red));
	}
	else if (color == 2)
	{
		red = (int)(9 * (1 - n) * pow(n, 3) * 255);
		green = (int)(15 * pow((1 - n), 2) * pow(n, 2) * 255);
		blue = (int)(8.5 * pow((1 - n), 3) * n * 255);
		return (set_colors(0, blue, red, green));
	}
	else if (color == 3)
	{
		red = (int)(9 * (1 - n) * pow(n, 3) * 255);
		green = (int)(15 * pow((1 - n), 2) * pow(n, 2) * 255);
		blue = (int)(8.5 * pow((1 - n), 3) * n * 255);
		return (set_colors(0, red, green, blue));
	}
}



__kernel void draw(__global int *data, t_fractal f)
{
	int			iteration;
	int			x;
	int			y;
	//int 		b;
	//double		t;
	//double		red;
	//double		green;
	//double		blue;
	//double		size;
	
	//size = f.threads;

	int gid = get_global_id(0);

	double		zre;
	double		zim;

    x = gid % WIN_X;
	y = gid / WIN_Y;

	// y = (int)((WIN_Y / size) * gid);
	// b = (int)((WIN_Y / size) * (gid + 1));
	// while (y <= b)
	// {
		f.c.im = f.max.im - y * f.factor.im;
		// x = 0;
		// while (x < WIN_X)
		// {
			f.c.re = f.min.re + x *	f.factor.re;
			f.z.re = f.c.re;
			f.z.im = f.c.im;
			iteration = 0;
			while ((f.z.re * f.z.re) + (f.z.im * f.z.im) < 4
					&& iteration < f.max_iteration)
			{
				zre = f.z.re;
				zim = f.z.im;
				f.z.im = 2 * zim * zre + f.c.im;
				f.z.re = (zre * zre) - (zim * zim) + f.c.re;

				++iteration;
			}
			// t = (double)iteration /
			// 	(double)f.max_iteration;

			// red = (int)(9 * (1 - t) * pow(t, 3) * 255);
			// green = (int)(15 * pow((1 - t), 2) * pow(t, 2) * 255);
			// blue = (int)(8.5 * pow((1 - t), 3) * t * 255);
			//Установка цвета точки
			// data[WIN_X * y + x] = red * green * blue;
			//data[WIN_X * y + x] = iteration % 256 << 1 + iteration % 0 << 3;;

			if (iteration <  f.max_iteration)
	    	    ((__global int *)data)[gid] = choose_color(iteration, f.max_iteration, 0);
	    	else
	            ((__global int *)data)[gid] = 0;
		// 	x++;
		// }
	// 	y++;
	// }

}


	 

	//  int gid = get_global_id(0);
	//double maxre, double maxim, double minre, double minim,
	//						int max_iteration, double size, double kre, double kim)

	// int			iteration;
	// int			x;
	// int			y;

	// double		t;
	// double		red;
	// double		green;
	// double		blue;
	// double 		factorre;
	// double 		factorim;
	// double 		cre;
	// double 		cim;
	// double 		zre;
	// double 		zim;


	// factorre = (maxre - minre) / (WIN_X - 1);
	// factorim = (maxim - minim) / (WIN_Y - 1);

	// y = (int)((WIN_Y / size) * gid);
	// while (y <= (int)((WIN_Y / size) * (gid + 1)))
	// {
	// 	cim = maxim - y * factorim;
	// 	x = 0;
	// 	while (x < WIN_X)
	// 	{
	// 		cre = minre + x * factorre;
	// 		zre = cre; 
	// 		zim = cim;
	// 		iteration = 0;
	// 		while ((zre * zre) + (zim * zim) <= 4 && iteration < max_iteration)
	// 		{
	// 			zre = (zre * zre) - (zim * zim) + kre;
	// 			zim	= 2.0 * zre * zim + kim;
	// 			iteration++;
	// 		}
	// 		t = (double)iteration / (double)max_iteration;

	// 		red = (int)(9 * (1 - t) * (t * t * t) * 255);
	// 		green = (int)(15 * ((1 - t) * (1 - t)) * (t * t) * 255);
	// 		blue = (int)(8.5 * ((1 - t) * (1 - t) * (1 - t)) * t * 255);

	// 		data[WIN_X * y + x] =  red * green * blue;
			
	// 		x++;
	// 	}
	// 	y++;
	// }

# define WIN_X 1000
# define WIN_Y 1000

#include "cl.h"

__kernel void draw(__global int *data, t_fractal f)
{
		int			iteration;
		int			x;
		int			y;
		double		t;
		double		red;
		double		green;
		double		blue;
		double		size;
		
		size = f.threads;

		int gid = get_global_id(0);

		f.factor.re = (f.max.re - f.min.re) / (WIN_X - 1);
		f.factor.im = (f.max.im - f.min.im) / (WIN_Y - 1);

		y = (int)((WIN_Y / size) * gid);
		while (y <= (int)((WIN_Y / size) * (gid + 1)))
		{
			f.c.im = f.max.im - y * f.factor.im;
			x = 0;
			while (x < WIN_X)
			{
				f.c.re = f.min.re + x *	f.factor.re;
				f.z.re = f.c.re;
				f.z.im = f.c.im;
				iteration = 0;
				while (pow(f.z.re, 2.0) +
						pow(f.z.im, 2.0) <= 4
						&& iteration < f.max_iteration)
				{
					f.z.re = pow(f.z.re, 2.0) - pow(f.z.im, 2.0) + f.c.re;
					f.z.im = 2.0 * f.z.re * f.z.im + f.c.im;
					iteration++;
				}
				t = (double)iteration /
					(double)f.max_iteration;

				red = (int)(9 * (1 - t) * pow(t, 3) * 255);
				green = (int)(15 * pow((1 - t), 2) * pow(t, 2) * 255);
				blue = (int)(8.5 * pow((1 - t), 3) * t * 255);
				//Установка цвета точки
				data[WIN_X * y + x] = red * green * blue;
				x++;
			}
			y++;
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
}
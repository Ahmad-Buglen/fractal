# define WIN_X 1000
# define WIN_Y 1000


__kernel void draw(__global int *data, double maxre, double maxim, double minre, double minim, int max_iteration)
{
	int			iteration;
	int			x;
	int			y;
	double		t;
	double		red;
	double		green;
	double		blue;
	double 		factorre;
	double 		factorim;
	double 		cre;
	double 		cim;
	double 		zre;
	double 		zim;

	factorre = (maxre - minre) / (WIN_X - 1);
	factorim = (maxim - minim) / (WIN_Y - 1);

	y = 0;
	while (y < WIN_Y)
	{
		cim = maxim - y * factorim;
		x = 0;
		while (x < WIN_X)
		{
			cre = minre + x * factorre;
			zre = cre; 
			zim = cim;
			iteration = 0;
			while ((zre * zre) + (zim * zim) <= 4 && (iteration < max_iteration))
			{
				zre = (zre * zre) - (zim * zim) + cre;
				zim	= 2.0 * zre * zim + cim;
				iteration++;
			}
			t = (double)iteration / (double)max_iteration;

			red = (int)(9 * (1 - t) * (t * t * t) * 255);
			green = (int)(15 * ((1 - t) * (1 - t)) * (t * t) * 255);
			blue = (int)(8 * ((1 - t) * (1 - t) * (1 - t)) * t * 255);

			data[WIN_X * y + x] = red * green * blue;
			x++;
		}
		y++;
	}
}
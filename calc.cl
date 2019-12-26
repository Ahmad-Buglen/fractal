# define WIN_X 1000
# define WIN_Y 1000


__kernel void draw(__global int *buf, double maxre, double maxim, double minre, double minim, int max_iteration, int message)
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

	int gid = get_global_id(0);

	message += gid;

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
			blue = (int)(8.5 * ((1 - t) * (1 - t) * (1 - t)) * t * 255);

			buf[WIN_X * y + x] =  red * green * blue;
			
			x++;
		}
		y++;
	}
}
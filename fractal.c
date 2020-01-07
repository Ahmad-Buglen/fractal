/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <admin@21-school.ru>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 20:45:56 by dphyliss          #+#    #+#             */
/*   Updated: 2019/12/23 13:04:54 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <math.h>
# include "mlx.h"

# ifdef __APPLE__
#  include <OpenCL/opencl.h>
# else
#  include <CL/cl.h>
# endif

# include "libft/libft.h"

# include "cl.h"

typedef	struct	s_catalog
{
	int			number;
	char		*name;
}				t_catalog;

typedef	struct	s_point
{
	int			x;
	int			y;
}				t_point;

typedef	struct	s_image
{
	int			*ptr;
	int			*data;
	int			bpp;
	int			size_line;
	int			endian;
}				t_image;

typedef struct		s_opcl
{
	cl_device_id		dev;
	cl_command_queue	queue;
	cl_context			context;
	cl_program			program;
	cl_kernel			kernel;

	cl_mem				buf;
	size_t				total_s;
	size_t				local_s;
	cl_platform_id		platformid;
	cl_uint				pltf_num;
	cl_uint				dev_num;
}					t_opcl;


typedef	struct	s_screen
{
	void		*mlx_ptr;
	void		*win_ptr;
	int			height;
	int			width;
	t_image		image;
	t_fractal	fractal;
	t_opcl		opcl;
	int 		info;
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

void		free_cl(t_screen *s)
{
	clReleaseContext(s->opcl.context);
	clReleaseCommandQueue(s->opcl.queue);
	clReleaseProgram(s->opcl.program);
	clReleaseKernel(s->opcl.kernel);
	clReleaseMemObject(s->opcl.buf);
}

void		terminate(t_screen *s, char *str)
{
	ft_putstr(str);
//	if (flag == CL_ERROR)
//		ft_putendl("OpenCL Error");
//	else if (flag == STD_ERROR)
//		ft_putendl("File read Error");
	free_cl(s);
	exit(0);
}

char	*load_src(char *path)
{
	int		fd;
	char	buf[100];
	char	*tmp;
	char	*src;

	src = 0;
	if ((fd = open(path, O_RDONLY)) < 0)
		return (0);
	while (read(fd, buf, 99) > 0)
	{
		if (!src)
			src = ft_strdup(buf);
		else
		{
			tmp = src;
			src = ft_strjoin(tmp, buf);
			free(tmp);
		}
		ft_bzero(buf, 100);
	}
	return (src);
}



void	get_kernel(t_screen *s)
{
	int		ret;
	char	buf[65536];
	size_t 	len;

	ret = clBuildProgram(s->opcl.program, 0,
			NULL, NULL, NULL, NULL);
	if (CL_SUCCESS != ret)
	{
		clGetProgramBuildInfo(s->opcl.program, s->opcl.dev,
				CL_PROGRAM_BUILD_LOG, sizeof(buf), buf, &len);
		ft_putstr(buf);
		terminate(s, "Error: Failed to build program executable.");
	}
	if (!(s->opcl.kernel = clCreateKernel(s->opcl.program,
			s->fractal.function, &ret)))
		terminate(s, "Error: Failed to create compute kernel.");
}

int		get_device(t_screen *s)
{
	int					ret;
	cl_platform_id		platformid;
	cl_uint				pltf_num;
	cl_uint				dev_num;

	if ((ret = clGetPlatformIDs(0, 0, &pltf_num)) != CL_SUCCESS)
		terminate(s, "Error: At obtain the list of platforms available.");
	if ((ret |= clGetPlatformIDs(pltf_num, &platformid, 0)) != CL_SUCCESS)
		terminate(s, "Error: At obtain the list of platforms available.");
	if ((ret |= clGetDeviceIDs(platformid,
			CL_DEVICE_TYPE_GPU, 0, 0, &dev_num)) != CL_SUCCESS)
		terminate(s, "Error: Failed to create a device group.");
	if ((ret |= clGetDeviceIDs(platformid,
			CL_DEVICE_TYPE_GPU, dev_num, &s->opcl.dev, 0)) != CL_SUCCESS)
		terminate(s, "Error: Failed to create a device group.");
	return (1);
}

int		init_cl(t_screen *s)
{
	int			ret;
	char		*src;

	if (!(get_device(s)))
		return (0);
	if (!(s->opcl.context = clCreateContext(0, 1, &s->opcl.dev, NULL, NULL, &ret)))
		terminate(s, "Error: Failed to create a compute context.");
	if (!(s->opcl.queue = clCreateCommandQueue(s->opcl.context,
			s->opcl.dev, 0, &ret)))
		terminate(s, "Error: Failed to create a command commands.");
	if (!(src = load_src("calc.cl")))
		terminate(s, "Error: when reading a program.");
	if (!(s->opcl.program =	clCreateProgramWithSource(s->opcl.context,
					1, (const char**)&src, NULL, &ret)))
		terminate(s, "Error: Failed to create compute program.");
	free(src);
	get_kernel(s);
	s->opcl.buf = clCreateBuffer(s->opcl.context,
		CL_MEM_READ_ONLY, sizeof(int) * WIN_X * WIN_Y, NULL, NULL);
	if (!s->opcl.buf)
    {
        printf("Error: Failed to allocate device memory!\n");
        exit(1);
    }    
	return (1);
}



void		set_args(t_screen *s)
{
	cl_int		ret;

	ret = clSetKernelArg(s->opcl.kernel, 0, sizeof(cl_mem), (void *)&s->opcl.buf);
	ret = clSetKernelArg(s->opcl.kernel, 1, sizeof(t_fractal), (void *)&s->fractal);
	if (ret != CL_SUCCESS)
		terminate(s, "Error: At set the arguments values for kernel.");
}

void		execute_kernel(t_screen *s)
{
	int		ret;
	s->opcl.total_s = WIN_X * WIN_Y;
	set_args(s);

	ret = clGetKernelWorkGroupInfo( s->opcl.kernel, s->opcl.dev, CL_KERNEL_WORK_GROUP_SIZE,
	 		sizeof(s->opcl.local_s), &s->opcl.local_s, NULL);
    if (ret != CL_SUCCESS)
    {
        printf("Error: Failed to retrieve kernel work group info. %d\n", ret);
        exit(1);
    }

	ret = clEnqueueNDRangeKernel(s->opcl.queue, s->opcl.kernel, 1,
			NULL, &s->opcl.total_s , &s->opcl.local_s, 0, NULL, NULL);
	if (ret != CL_SUCCESS)
		terminate(s, "Error: Failed to execute kernel.");
}

void	ft_menu(t_screen *const s)
{
	mlx_string_put(s->mlx_ptr, s->win_ptr, WIN_Y / 20, 5,
			0xFC1414, "Instruction:");
	mlx_string_put(s->mlx_ptr, s->win_ptr, 20, 25, 0x7DCCBD, 
		ft_strjoin("fractal name: ", s->fractal.function));
	mlx_string_put(s->mlx_ptr, s->win_ptr, 20, 50, 0x4FA833, 
		ft_strjoin("iteration count: ", ft_itoa(s->fractal.max_iteration)));
	mlx_string_put(s->mlx_ptr, s->win_ptr, 20, 75,
			0xFFAB59, "N(next), P(prev) change fractal");
	mlx_string_put(s->mlx_ptr, s->win_ptr, 20, 100,
			0xFA4F29, "key C for change color;");
	mlx_string_put(s->mlx_ptr, s->win_ptr, 20, 125,
			0x1A7A63, "srrows for move;");
	mlx_string_put(s->mlx_ptr, s->win_ptr, 20, 150,
			0xFF7521, "scroll for zoom;");
	mlx_string_put(s->mlx_ptr, s->win_ptr, 20, 175,
			0xC98721, "space for return to basic;");
	mlx_string_put(s->mlx_ptr, s->win_ptr, 20, 200,
			0x9C4529, "key i for remove menu;");
	mlx_string_put(s->mlx_ptr, s->win_ptr, 20, 225,
		0x389482, "+/- for change max iteration;");
	mlx_string_put(s->mlx_ptr, s->win_ptr, 20, 250,
			0xBA2E21, "key esc for exit.");
}

void		draw(t_screen *s)
{
	ft_bzero(s->image.data, WIN_X * WIN_Y * 4);
	mlx_clear_window(s->mlx_ptr, s->win_ptr);

	int		err = 0;
	
	s->fractal.factor.re = (s->fractal.max.re - s->fractal.min.re) / (WIN_X - 1);
	s->fractal.factor.im = (s->fractal.max.im - s->fractal.min.im) / (WIN_Y - 1);

	execute_kernel(s);

	clEnqueueReadBuffer(s->opcl.queue, s->opcl.buf, CL_TRUE, 0,
			sizeof(int) * WIN_X * WIN_Y, s->image.data, 0, NULL, NULL);
	if (err != CL_SUCCESS)
    {
        printf("Error: Failed to read output array! %d\n", err);
        exit(1);
    }	
    clFlush(s->opcl.queue);
	clFinish(s->opcl.queue);

	mlx_put_image_to_window(s->mlx_ptr, s->win_ptr, s->image.ptr, 0, 0);
	if (s->info)
		ft_menu(s);
	//print_info(f);

	//mlx_do_sync(s->mlx_ptr); //???
}

int				julia_motion(int x, int y, t_screen *s)
{
	if (!s->fractal.fixed)
	{
		s->fractal.k.re = 4 * ((double)x / WIN_X - 0.5),
		s->fractal.k.im	= 4 * ((double)(WIN_Y - y) / WIN_Y - 0.5);
		draw(s);
	}
	return (0);
}

void 	reset(t_screen * s)
{
	s->fractal.min = init_compl(-2.0, -2.0);
	s->fractal.max.re = 2.0;
	s->fractal.max.im = s->fractal.min.im +
		(s->fractal.max.re - s->fractal.min.re) * WIN_Y / WIN_X;
	s->fractal.max_iteration = 50;
	if (2 == s->fractal.number)
	{
		s->fractal.k = init_compl(-0.4, 0.6);
		s->fractal.fixed = 0;
	}
	s->fractal.color = 0;
}

void	init(t_screen * s)
{
	if (!(s->mlx_ptr = mlx_init()))
		ft_exit("Error: mlx failed.\n");
	if (!(s->win_ptr = mlx_new_window(s->mlx_ptr,
					WIN_X, WIN_Y, "asdf")))
		ft_exit("Error: mlx_new_window failed.\n");
	
	if (!(s->image.ptr = mlx_new_image(s->mlx_ptr, WIN_X, WIN_Y)))
		ft_exit("Error: mls_new_image failed.\n");
	if (!(s->image.data = (int *)mlx_get_data_addr(s->image.ptr,
		&s->image.bpp, &s->image.size_line, &s->image.endian)))
		ft_exit("Error: mls_new_image failed.\n");
	s->info = 0;
	s->fractal.color = 0;
	reset(s);
}

void draw1(t_screen * const s)
{
	int			iteration;
	int			x;
	int			y;
	double		t;
	double		red;
	double		green;
	double		blue;

	s->fractal.factor = init_compl(
			(s->fractal.max.re - s->fractal.min.re) / (WIN_X - 1),
			(s->fractal.max.im - s->fractal.min.im) / (WIN_Y - 1));

	y = 0;
	while (y < WIN_Y)
	{
		s->fractal.c.im = s->fractal.max.im - y *
			s->fractal.factor.im;
		x = 0;
		while (x < WIN_X)
		{
			s->fractal.c.re = s->fractal.min.re + x *
				s->fractal.factor.re ;
			s->fractal.z = init_compl(s->fractal.c.re,
					s->fractal.c.im);
			iteration = 0;
			while (pow(s->fractal.z.re, 2.0) +
					pow(s->fractal.z.im, 2.0) <= 4
					&& iteration < s->fractal.max_iteration)
			{
				// s->fractal.z = init_compl(
				// 		pow(s->fractal.z.re, 2.0) -
				// 		pow(s->fractal.z.im, 2.0) + s->fractal.c.re,
				// 		2.0 * s->fractal.z.re * s->fractal.z.im +
				// 		s->fractal.c.im);
				s->fractal.z = init_compl(
					pow(s->fractal.z.re, 2.0) -
					pow(s->fractal.z.im, 2.0) + s->fractal.c.re,
					2.0 * s->fractal.z.re * s->fractal.z.im +
					s->fractal.c.im);
				iteration++;
			}
			t = (double)iteration /
				(double)s->fractal.max_iteration;

			red = (int)(9 * (1 - t) * pow(t, 3) * 255);
			green = (int)(15 * pow((1 - t), 2) * pow(t, 2) * 255);
			blue = (int)(8.5 * pow((1 - t), 3) * t * 255);
			//Установка цвета точки
			s->image.data[WIN_X * y + x] = red * green * blue;
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(s->mlx_ptr, s->win_ptr,
						s->image.ptr, 0, 0);
}



void set_fractal(t_screen *s, int number)
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


double	interpolate(double start, double end, double interpolation)
{
	return (start + ((end - start) * interpolation));
}

int		mouse_hook(const  int keycode, int x, int y,	t_screen * const s)
{
	t_compl		mouse;
	double		interpolation;
	double		zoom;

	if (keycode == 4 || keycode == 5)
	{
		mouse = init_compl(
			(double)x / (WIN_X /
				(s->fractal.max.re - s->fractal.min.re))
				+ s->fractal.min.re,
			(double)y /
			(WIN_Y / (s->fractal.max.im - s->fractal.min.im))
				* -1 + s->fractal.max.im);
		if (keycode == 4)
			zoom = 0.80;
		else
			zoom = 1.20;
		interpolation = 1.0 / zoom;
		s->fractal.min.re =
			interpolate(mouse.re, s->fractal.min.re, interpolation);
		s->fractal.min.im =
			interpolate(mouse.im, s->fractal.min.im, interpolation);
		s->fractal.max.re =
			interpolate(mouse.re, s->fractal.max.re, interpolation);
		s->fractal.max.im =
			interpolate(mouse.im, s->fractal.max.im, interpolation);
			draw(s);
	}
	return (1);
}

static void	move(int key, t_screen *s)
{
	t_compl	delta;

	delta = init_compl(ft_abs(s->fractal.max.re - s->fractal.min.re),
		ft_abs(s->fractal.max.im - s->fractal.min.im));
	if (key == 123) // left
	{
		s->fractal.min.re -= delta.re * 0.05;
		s->fractal.max.re -= delta.re * 0.05;
	}
	else if (key == 124) // right
	{
		s->fractal.min.re += delta.re * 0.05;
		s->fractal.max.re += delta.re * 0.05;
	}
	else if (key == 126) // up
	{
		s->fractal.min.im += delta.im * 0.05;
		s->fractal.max.im += delta.im * 0.05;
	}
	else if (key == 125) // down
	{
		s->fractal.min.im -= delta.im * 0.05;
		s->fractal.max.im -= delta.im * 0.05;
	}
	draw(s);
}

int     key_hook(const  int keycode, t_screen * const s)
{
	int ret;

	printf("keycode = %d\n", keycode);
	if (53 == keycode)
		exit(0);
	else if (69 == keycode)
	{
		s->fractal.max_iteration += 1;
		draw(s);
	}
	else if (78 == keycode)
	{
		s->fractal.max_iteration -= 1;
		draw(s);
	}
	else if (3 == keycode)
	{
		s->fractal.fixed = (s->fractal.fixed) ? 0 : 1;
	}
	else if (8 == keycode)
	{
		if (3 == s->fractal.color)
			s->fractal.color = 0;
		else 
			s->fractal.color += 1;
		draw(s);
	}
	else if (34 == keycode)
	{
		s->info = (s->info) ? 0 : 1;
		draw(s);
	}
	else if (35 == keycode)
	{
		reset(s);
		if (1 == s->fractal.number)
			s->fractal.number = 10;
		else 
			s->fractal.number -= 1;
		set_fractal(s, s->fractal.number);
		if (!(s->opcl.kernel = clCreateKernel(s->opcl.program,
			s->fractal.function, &ret)))
			terminate(s, "Error: Failed to create compute kernel.");
		draw(s);
	}
	else if (45 == keycode)
	{
		reset(s);
		if (10 == s->fractal.number)
			s->fractal.number = 1;
		else 
			s->fractal.number += 1;
		set_fractal(s, s->fractal.number);
		if (!(s->opcl.kernel = clCreateKernel(s->opcl.program,
			s->fractal.function, &ret)))
			terminate(s, "Error: Failed to create compute kernel.");
		draw(s);
	}
	else if (49 == keycode)
	{
		reset(s);
		draw(s);
	}
	else if ((keycode >= 123) && (keycode <= 126))
		move(keycode, s);
	return (1);
}

void	ft_usage(int ac, char **av)
{	
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

int		main(int ac, char **av)
{
	t_screen	*s;
	int			fd;

	if ((2 != ac) || ((ft_atoi(av[1]) < 1) || (ft_atoi(av[1]) > 10))) 
		ft_usage(ac, av);

	if (!(s = (t_screen *)malloc(sizeof(t_screen))))
		ft_exit("Error: the memory hasn't been allocated.\n");
	set_fractal(s, ft_atoi(av[1]));




	init(s);
		 init_cl(s);
	 draw(s);

//?	mlx_hook(fractol->window, 17, 0, close, fractol);
	
	mlx_hook(s->win_ptr, 2, 0, key_hook, s);

	mlx_hook(s->win_ptr, 6, 0, julia_motion, s); // утечка?
	mlx_hook(s->win_ptr, 4, 0, mouse_hook, s);
	mlx_loop(s->mlx_ptr);
		write(1, "here", 4);
	return (1);
}

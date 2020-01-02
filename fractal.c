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

# define WIN_X 1000
# define WIN_Y 1000

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

typedef struct	s_fractal
{
	int			max_iteration;
	t_compl		min;
	t_compl		max;
	t_compl		factor;
	t_compl		c;
	t_compl		z;
	t_compl		k;
	char		fixed;
	int 		threads;		
}				t_fractal;

typedef	struct	s_screen
{
	void		*mlx_ptr;
	void		*win_ptr;
	int			height;
	int			width;
	t_image		image;
	t_fractal	fractal;
	t_opcl		opcl;
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
	size_t len;

	ret = clBuildProgram(s->opcl.program, 0,
			NULL, NULL, NULL, NULL);
	if (CL_SUCCESS != ret)
	{
		clGetProgramBuildInfo(s->opcl.program, s->opcl.dev,
				CL_PROGRAM_BUILD_LOG, sizeof(buf), buf, &len);
		ft_putstr(buf);
		terminate(s, "Error: Failed to build program executable!");
	}
	if (!(s->opcl.kernel = clCreateKernel(s->opcl.program,
			"draw", &ret)))
		terminate(s, "Error: Failed to create compute kernel!");
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
	if (!(s->opcl.program =
			clCreateProgramWithSource(s->opcl.context,
					1, (const char**)&src, NULL, &ret)))
		terminate(s, "Error: Failed to create compute program.");
	free(src);

		get_kernel(s);

		int memLenth = 10;

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

	double size = 256;
	ret = clSetKernelArg(s->opcl.kernel, 0, sizeof(cl_mem), &s->opcl.buf);

	ret = clSetKernelArg(s->opcl.kernel, 1, sizeof(t_fractal), (void *)&s->fractal);
	// ret |= clSetKernelArg(s->opcl.kernel, 1, sizeof(double), &s->fractal.max.re);
	// ret |= clSetKernelArg(s->opcl.kernel, 2, sizeof(double), &s->fractal.max.im);
	// ret |= clSetKernelArg(s->opcl.kernel, 3, sizeof(double), &s->fractal.min.re);
	// ret |= clSetKernelArg(s->opcl.kernel, 4, sizeof(double), &s->fractal.min.im);
	// ret |= clSetKernelArg(s->opcl.kernel, 5, sizeof(int), &s->fractal.max_iteration);
	// ret |= clSetKernelArg(s->opcl.kernel, 6, sizeof(double), &size);
	// ret |= clSetKernelArg(s->opcl.kernel, 7, sizeof(double), &s->fractal.k.re);
	// ret |= clSetKernelArg(s->opcl.kernel, 8, sizeof(double), &s->fractal.k.im);
	if (ret != CL_SUCCESS)
		terminate(s, "Error: At set the arguments values for kernel.");
}

void		execute_kernel(t_screen *s)
{
	int		ret;
	//s->opcl.total_s = WIN_X * WIN_Y;
//	s->opcl.local_s;		
	const size_t globalT = 256;
	set_args(s);
//**?
	int		err = 0;
	 err = clGetKernelWorkGroupInfo( s->opcl.kernel, s->opcl.dev, CL_KERNEL_WORK_GROUP_SIZE,
	 		sizeof(s->opcl.local_s), &s->opcl.local_s, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to retrieve kernel work group info. %d\n", err);
        exit(1);
    }

	ret = clEnqueueNDRangeKernel(s->opcl.queue, s->opcl.kernel, 1,
			NULL, &globalT , &s->opcl.local_s, 0, NULL, NULL);
	if (ret != CL_SUCCESS)
		terminate(s, "Error: Failed to execute kernel.");
}


void		draw(t_screen *s)
{

	ft_bzero(s->image.data, WIN_X * WIN_Y * 4);
	mlx_clear_window(s->mlx_ptr, s->win_ptr);

	int		err = 0;

	execute_kernel(s);

	clEnqueueReadBuffer(s->opcl.queue, s->opcl.buf, CL_TRUE, 0,
			sizeof(int) * WIN_X * WIN_Y, s->image.data, 0, NULL, NULL);
	if (err != CL_SUCCESS)
    {
        printf("Error: Failed to read output array! %d\n", err);
        exit(1);
    }

	mlx_put_image_to_window(s->mlx_ptr, s->win_ptr, s->image.ptr, 0, 0);
	//print_info(f);
	mlx_do_sync(s->mlx_ptr); //???
}

// 	//аргументы по ссылке или по значению передавать 
// 	// по два вызова функций для чего 
// 	// преобразование типов на вход и выход kernel
// 	// написать простейший код 
// 	// в чем разница cl типов 

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
	s->fractal.min.re = -2.0;
	s->fractal.max.re = 2.0;
	s->fractal.min.im = -2.0;
	s->fractal.max.im = s->fractal.min.im +
		(s->fractal.max.re - s->fractal.min.re) * WIN_Y / WIN_X;
	s->fractal.max_iteration = 25;

	s->fractal.k = init_compl(-0.4, 0.6);
	s->fractal.threads = 256;
	s->fractal.fixed = 0;
}

// void print_fractal(t_screen * const s)
// {
// 	int			iteration;
// 	int			x;
// 	int			y;

// 	s->fractal.factor = init_compl(
// 			(s->fractal.max.re - s->fractal.min.re) / (WIN_X - 1),
// 			(s->fractal.max.im - s->fractal.min.im) / (WIN_Y - 1));

// 	y = 0;
// 	while (y < WIN_Y)
// 	{
// 		s->fractal.c.im = s->fractal.max.im - y *
// 			s->fractal.factor.im;
// 		x = 0;
// 		while (x < WIN_X)
// 		{
// 			s->fractal.c.re = s->fractal.min.re + x *
// 				s->fractal.factor.re ;
// 			s->fractal.z = init_compl(s->fractal.c.re,
// 					s->fractal.c.im);
// 			iteration = 0;
// 			while (pow(s->fractal.z.re, 2.0) +
// 					pow(s->fractal.z.im, 2.0) <= 4
// 					&& iteration < s->fractal.max_iteration)
// 			{
// 				s->fractal.z = init_compl(
// 						pow(s->fractal.z.re, 2.0) -
// 						pow(s->fractal.z.im, 2.0) + s->fractal.c.re,
// 						2.0 * s->fractal.z.re * s->fractal.z.im +
// 						s->fractal.c.im);
// 				iteration++;
// 			}
// 			s->fractal.t = (double)iteration /
// 				(double)s->fractal.max_iteration;

// 			s->fractal.red = (int)(9 * (1 - s->fractal.t) *
// 					pow(s->fractal.t, 3) * 255);
// 			s->fractal.green = (int)(15 * pow((1 - s->fractal.t), 2) *
// 					pow(s->fractal.t, 2) * 255);
// 			s->fractal.blue = (int)(8.5 * pow((1 - s->fractal.t), 3) *
// 					s->fractal.t * 255);
// 			//Установка цвета точки
// 			s->image.data[WIN_X * y + x] = s->fractal.red *
// 				s->fractal.green * s->fractal.blue;
// 			x++;
// 		}
// 		y++;
// 	}
// 	mlx_put_image_to_window(s->mlx_ptr, s->win_ptr,
// 						s->image.ptr, 0, 0);
// }

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
		s->fractal.fixed = 1;
	}
	else if ((keycode >= 123) && (keycode <= 126))
	{
		move(keycode, s);
	}
	return (1);
}

int		main(int ac, char **av)
{
	t_screen	*s;
	int			fd;

//	if (2 != ac)
//		ft_exit("1");
	if (!(s = (t_screen *)malloc(sizeof(t_screen))))
		ft_exit("Error: the memory hasn't been allocated.\n");
	init(s);
	init_cl(s);
	draw(s);

//?	mlx_hook(fractol->window, 17, 0, close, fractol);
	
	mlx_hook(s->win_ptr, 2, 0, key_hook, s);
	mlx_hook(s->win_ptr, 4, 0, mouse_hook, s);
	mlx_loop(s->mlx_ptr);
		mlx_hook(s->win_ptr, 6, 0, julia_motion, s);
		write(1, "here", 4);
	return (1);
}

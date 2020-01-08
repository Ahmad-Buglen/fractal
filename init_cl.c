/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cl.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <admin@21-school.ru>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 19:09:33 by dphyliss          #+#    #+#             */
/*   Updated: 2020/01/08 19:09:36 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractal.h"

int 	ft_close(void *const s)
{
	free_cl(s);
	exit(0);
}

char	*load_src(const char *const path)
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

void	get_kernel(t_screen *const s)
{
	int		ret;
	if (s->opcl.kernel)
		clReleaseKernel(s->opcl.kernel);
	if (!(s->opcl.kernel = clCreateKernel(s->opcl.program,
			s->fractal.function, &ret)))
		terminate(s, "Error: Failed to create compute kernel.");
}

void	get_device(t_screen *const s)
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
	if (!(s->opcl.context = clCreateContext(0, 1, &s->opcl.dev,
												NULL, NULL, &ret)))
		terminate(s, "Error: Failed to create a compute context.");
	if (!(s->opcl.queue = clCreateCommandQueue(s->opcl.context,
			s->opcl.dev, 0, &ret)))
		terminate(s, "Error: Failed to create a command commands.");
}

int 	init_cl(t_screen *const s)
{
	int			ret;
	char		*src;
	char 		buf[65536];
	size_t 		len;

	get_device(s);
	if (!(src = load_src("calc.cl")))
		terminate(s, "Error: when reading a program.");
	if (!(s->opcl.program =	clCreateProgramWithSource(s->opcl.context,
					1, (const char**)&src, NULL, &ret)))
		terminate(s, "Error: Failed to create compute program.");
	free(src);
	ret = clBuildProgram(s->opcl.program, 0,
			NULL, NULL, NULL, NULL);
	if (CL_SUCCESS != ret)
	{
		clGetProgramBuildInfo(s->opcl.program, s->opcl.dev,
				CL_PROGRAM_BUILD_LOG, sizeof(buf), buf, &len);
		ft_putstr(buf);
		terminate(s, "Error: Failed to build program executable.");
	}
	get_kernel(s);
	if (!(s->opcl.buf = clCreateBuffer(s->opcl.context,
		CL_MEM_READ_ONLY, sizeof(int) * WIN_X * WIN_Y, NULL, NULL)))
        terminate(s, "Error: Failed to allocate device memory.");
	return (1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <admin@21-school.ru>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 19:09:43 by dphyliss          #+#    #+#             */
/*   Updated: 2020/01/08 19:09:46 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractal.h"

void	execute_kernel(t_screen *const s)
{
	int		ret;

	ret = clSetKernelArg(s->opcl.kernel, 0, sizeof(cl_mem),
							(void *)&s->opcl.buf);
	ret = clSetKernelArg(s->opcl.kernel, 1, sizeof(t_fractal),
							(void *)&s->fractal);
	ret = clSetKernelArg(s->opcl.kernel, 2, sizeof(char),
							(void *)&s->fractal.color);
	if (ret != CL_SUCCESS)
		terminate(s, "Error: At set the arguments values for kernel.");
	ret = clGetKernelWorkGroupInfo(s->opcl.kernel, s->opcl.dev,
						CL_KERNEL_WORK_GROUP_SIZE, sizeof(s->opcl.local_s),
							&s->opcl.local_s, NULL);
	if (ret != CL_SUCCESS)
		terminate(s, "Error: Failed to retrieve kernel work group info.");
	ret = clEnqueueNDRangeKernel(s->opcl.queue, s->opcl.kernel, 1,
			NULL, &s->opcl.total_s, &s->opcl.local_s, 0, NULL, NULL);
	if (ret != CL_SUCCESS)
		terminate(s, "Error: Failed to execute kernel.");
}

void	draw(t_screen *const s)
{
	int		ret;

	ft_bzero(s->image.data, WIN_X * WIN_Y * 4);
	mlx_clear_window(s->mlx_ptr, s->win_ptr);
	s->fractal.factor = init_compl(
					(s->fractal.max.re - s->fractal.min.re) / (WIN_X - 1),
					(s->fractal.max.im - s->fractal.min.im) / (WIN_Y - 1));
	execute_kernel(s);
	ret = clEnqueueReadBuffer(s->opcl.queue, s->opcl.buf, CL_TRUE, 0,
			sizeof(int) * WIN_X * WIN_Y, s->image.data, 0, NULL, NULL);
	if (ret != CL_SUCCESS)
		terminate(s, "Error: Failed to read output array.");
	clFlush(s->opcl.queue);
	clFinish(s->opcl.queue);
	mlx_put_image_to_window(s->mlx_ptr, s->win_ptr, s->image.ptr, 0, 0);
	if (s->info)
		ft_menu(s);
}

void	ft_strput(const t_screen *const s, const int address,
					const int color, char *const str)
{
	mlx_string_put(s->mlx_ptr, s->win_ptr, 20, address, color, str);
}

void	ft_menu(const t_screen *const s)
{
	char *temp;
	char *buff;

	mlx_string_put(s->mlx_ptr, s->win_ptr, WIN_Y / 20, 5,
			0xFC1414, "Instruction:");
	temp = ft_strjoin("fractal name: ", s->fractal.function);
	ft_strput(s, 25, 0x7DCCBD, temp);
	free(temp);
	buff = ft_itoa(s->fractal.max_iteration);
	temp = ft_strjoin("iteration count: ", buff);
	free(buff);
	ft_strput(s, 50, 0x4FA833, temp);
	free(temp);
	ft_strput(s, 75, 0xFFAB59, "N(next), P(prev) change fractal");
	ft_strput(s, 100, 0xFA4F29, "key C for change color;");
	ft_strput(s, 125, 0x1A7A63, "srrows for move;");
	ft_strput(s, 150, 0xFF7521, "scroll for zoom;");
	ft_strput(s, 175, 0xC98721, "space for return to basic;");
	ft_strput(s, 200, 0x9C4529, "key i for remove menu;");
	ft_strput(s, 225, 0x389482, "+/- for change max iteration;");
	ft_strput(s, 250, 0xBA2E21, "key esc for exit.");
}

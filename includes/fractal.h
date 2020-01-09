/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractal.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <admin@21-school.ru>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 19:09:03 by dphyliss          #+#    #+#             */
/*   Updated: 2020/01/08 19:09:06 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTAL_H
# define FRACTAL_H

# include <stdlib.h>
# include <unistd.h>
# include "mlx.h"
# ifdef __APPLE__
#  include <OpenCL/opencl.h>
# else
#  include <CL/cl.h>
# endif
# include "../libft/libft.h"
# include "cl.h"

typedef	struct			s_point
{
	int					x;
	int					y;
}						t_point;

typedef	struct			s_image
{
	int					*ptr;
	int					*data;
	int					bpp;
	int					size_line;
	int					endian;
}						t_image;

typedef struct			s_opcl
{
	cl_device_id		dev;
	cl_command_queue	queue;
	cl_context			context;
	cl_program			program;
	cl_kernel			kernel;
	cl_mem				buf;
	size_t				total_s;
	size_t				local_s;
}						t_opcl;

typedef	struct			s_screen
{
	t_image				image;
	t_fractal			fractal;
	t_opcl				opcl;
	void				*mlx_ptr;
	void				*win_ptr;
	int					info;
	int					height;
	int					width;
}						t_screen;

void					draw(t_screen *const s);
void					ft_strput(const t_screen *const s, const int address,
									const int color, char *const str);
void					ft_menu(const t_screen *const s);
void					execute_kernel(t_screen *s);
void					init(t_screen *const s, const int init);
void					set_fractal(t_screen *const s, const int number);
void					ft_usage();
int						ft_close(void *const s);
char					*load_src(const char *const path);
void					get_kernel(t_screen *const s);
void					get_device(t_screen *const s);
int						init_cl(t_screen *const s);
void					terminate(t_screen *const s, const char *const str);
void					move(const int key, t_screen *const s);
void					key_hook_next(const int keycode, t_screen *const s);
int						key_hook(const int keycode, t_screen *const s);
void					ft_exit(const char *const str);
t_compl					init_compl(const double re, const double im);
void					free_cl(t_screen *const s);
int						mouse_hook(const int keycode, const int x, const int y,
									t_screen *const s);
int						julia_motion(const int x, const int y,
										t_screen *const s);

#endif

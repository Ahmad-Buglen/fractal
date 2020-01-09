/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <admin@21-school.ru>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 19:09:13 by dphyliss          #+#    #+#             */
/*   Updated: 2020/01/08 19:09:15 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractal.h"

void	terminate(t_screen *const s, const char *const str)
{
	ft_putendl(str);
	free_cl(s);
	exit(0);
}

void	move(const int key, t_screen *const s)
{
	t_compl	delta;

	delta = init_compl(ft_abs(s->fractal.max.re - s->fractal.min.re),
		ft_abs(s->fractal.max.im - s->fractal.min.im));
	if (key == 123)
	{
		s->fractal.min.re -= delta.re * 0.05;
		s->fractal.max.re -= delta.re * 0.05;
	}
	else if (key == 124)
	{
		s->fractal.min.re += delta.re * 0.05;
		s->fractal.max.re += delta.re * 0.05;
	}
	else if (key == 126)
	{
		s->fractal.min.im += delta.im * 0.05;
		s->fractal.max.im += delta.im * 0.05;
	}
	else if (key == 125)
	{
		s->fractal.min.im -= delta.im * 0.05;
		s->fractal.max.im -= delta.im * 0.05;
	}
	draw(s);
}

void	key_hook_next(const int keycode, t_screen *const s)
{
	if ((35 == keycode) || (45 == keycode))
	{
		if (35 == keycode)
			s->fractal.number = (1 == s->fractal.number) ?
								10 : s->fractal.number - 1;
		else
			s->fractal.number = (10 == s->fractal.number) ?
								1 : s->fractal.number + 1;
		set_fractal(s, s->fractal.number);
		init(s, 0);
		get_kernel(s);
		draw(s);
	}
	else if (49 == keycode)
	{
		init(s, 0);
		draw(s);
	}
	else if ((keycode >= 123) && (keycode <= 126))
		move(keycode, s);
}

int		key_hook(const int keycode, t_screen *const s)
{
	if (53 == keycode)
		exit(0);
	else if (8 == keycode)
	{
		s->fractal.color = (3 == s->fractal.color) ?
			0 : s->fractal.color + 1;
		draw(s);
	}
	else if ((69 == keycode) || (78 == keycode))
	{
		s->fractal.max_iteration = (69 == keycode) ?
			s->fractal.max_iteration + 10 : s->fractal.max_iteration - 10;
		draw(s);
	}
	else if (3 == keycode)
		s->fractal.fixed = (s->fractal.fixed) ? 0 : 1;
	else if (34 == keycode)
	{
		s->info = (s->info) ? 0 : 1;
		draw(s);
	}
	key_hook_next(keycode, s);
	return (1);
}

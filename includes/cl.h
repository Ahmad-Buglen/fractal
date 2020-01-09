/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <admin@21-school.ru>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 19:27:09 by dphyliss          #+#    #+#             */
/*   Updated: 2020/01/08 19:27:18 by dphyliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CL_H
# define CL_H

# define WIN_X 1024
# define WIN_Y 1024

typedef	struct	s_compl
{
	double		im;
	double		re;
}				t_compl;

typedef struct	s_fractal
{
	t_compl		min;
	t_compl		max;
	t_compl		factor;
	t_compl		c;
	t_compl		z;
	t_compl		k;
	int			max_iteration;
	int			number;
	char		fixed;
	char		julia;
	char		*function;
	char		color;
}				t_fractal;

#endif

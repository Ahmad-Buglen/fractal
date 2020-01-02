# define WIN_X 1000
# define WIN_Y 1000

typedef	struct	s_compl
{
	double			im;
	double			re;
}				t_compl;

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

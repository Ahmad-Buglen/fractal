# define WIN_X 1024
# define WIN_Y 1024

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
	char		julia;
	char		*function;
	int 		number;
	int 		color;
}				t_fractal;
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>


# include "libft/libft.h"


typedef	struct	s_image
{
	int			size_line;
}				t_image;

typedef struct		s_opcl
{
	int				local_s;
}					t_opcl;

typedef struct	s_fractal
{
	int			green;
}				t_fractal;

typedef	struct	s_screen
{
//	t_image		image;
//	t_fractal	fractal;
	t_opcl		opcl;
}				t_screen;

int main()
{
	t_screen 	*s = (t_screen *)malloc(sizeof(t_screen));
		printf("%d \n ", sizeof(s));
			s->opcl = (t_opcl *)malloc(sizeof(t_opcl));
				printf("%d \n ", sizeof(s->opcl));


	return (1);
}
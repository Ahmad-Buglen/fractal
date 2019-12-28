# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

# include "libft/libft.h"

typedef	struct	s_image
{
	int			size_line;
}				t_image;

typedef struct	s_opcl
{
	int			local_s;
}				t_opcl;

typedef struct	s_fractal
{
	int			green;
}				t_fractal;

typedef	struct	s_screen1
{
//	t_image		image;
//	t_fractal	fractal;
	t_opcl		opcl;
}				t_screen1;

typedef	struct	s_screen
{
//	t_image		image;
//	t_fractal	fractal;
		int			green;
	t_opcl		*opcl;
}				t_screen;

int main()
{
	t_screen t_s;
	printf("t_s %d \n ", sizeof(t_s));
	printf("t_s.opcl %d \n ", sizeof(t_s.opcl));
	t_screen *t_s1 = (t_screen *)malloc(sizeof(t_screen));
	printf("t_s %d \n ", sizeof(t_s1));
	//s->opcl = (t_opcl *)malloc(sizeof(t_opcl));
	//printf("%d \n ", sizeof(t_screen1->opcl));

	return (1);
}

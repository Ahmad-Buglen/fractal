# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dphyliss <admin@21-school.ru>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/01/09 12:34:53 by dphyliss          #+#    #+#              #
#    Updated: 2020/01/09 12:34:55 by dphyliss         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re

SRC = draw.c keyboard.c init_cl.c mouse.c fractal.c

OBJ = $(SRC:.c=.o)

NAME = fractal

LIB = libft/libft.a

MLX = ./mlx/libmlx.a

HEADER = ./includes/fractal.h

all: $(LIB) $(NAME)

$(LIB):
	make -C ./libft
$(MLX):
	make -C ./mlx
$(NAME): $(MLX) $(OBJ) includes/fractal.h
	gcc -Wall -Wextra -Werror -o $(NAME) -I $(HEADER) $(LIB) $(MLX) -lmlx -framework OpenGL -framework AppKit -framework OpenCL $(OBJ)
%.o: sources/%.c $(HEADER)
	gcc -c $<
clean:
	make clean -C ./libft
	rm -rf $(OBJ)
fclean: clean
	make fclean -C ./libft
	rm -rf $(NAME)
re: fclean all
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dphyliss <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/13 23:03:53 by dphyliss          #+#    #+#              #
#    Updated: 2019/11/30 16:30:22 by dphyliss         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

HEADER = libft.h

SRCS = ft_atoi.c	ft_lstadd.c	ft_memdel.c	ft_sqrt.c	ft_striteri.c	ft_strnstr.c \
	   ft_bzero.c	ft_lstdel.c	ft_memmove.c	ft_square.c	ft_strjoin.c	ft_strrchr.c \
	   ft_isalnum.c	ft_lstdelone.c	ft_memset.c	ft_strcat.c	ft_strlcat.c	ft_strsplit.c \
	   ft_isalpha.c	ft_lstiter.c	ft_putchar.c	ft_strchr.c	ft_strlen.c	ft_strstr.c \
	   ft_isascii.c	ft_lstmap.c	ft_putchar_fd.c	ft_strclr.c	ft_strmap.c	ft_strsub.c \
	   ft_isdigit.c	ft_lstnew.c	ft_putendl.c	ft_strcmp.c	ft_strmapi.c	ft_strtrim.c \
	   ft_ismaxint.c	ft_memalloc.c	ft_putendl_fd.c	ft_strcpy.c	ft_strncat.c \
	   ft_tolower.c	ft_abs.c ft_free_dual_char.c \
	   ft_ismaxlong.c	ft_memccpy.c	ft_putnbr.c	ft_strdel.c	ft_strncmp.c	ft_toupper.c \
	   ft_isminint.c	ft_memchr.c	ft_putnbr_fd.c	ft_strdup.c	ft_strncpy.c get_next_line.c \
	   ft_isprint.c	ft_memcmp.c	ft_putstr.c	ft_strequ.c	ft_strnequ.c ft_lstgnladd.c\
	   ft_itoa.c	ft_memcpy.c	ft_putstr_fd.c	ft_striter.c	ft_strnew.c	ft_strndup.c \

OBJECTS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJECTS) 
	ar rc $(NAME) $(OBJECTS)
	ranlib $(NAME)

%.o: %.c $(HEADER)
	gcc -Wall -Wextra -Werror -c $< 

.PHONY: clean fclean re

clean:
	/bin/rm -f $(OBJECTS)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: danicamp <danicamp@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/11/06 11:59:52 by danicamp          #+#    #+#              #
#    Updated: 2026/11/06 14:52:38 by danicamp         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = codexion

SRCS = coders.c codexion.c dongles.c \
	   parse_utils.c parsing.c time.c \
	   log.c routine.c simulator.c \

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
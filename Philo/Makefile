SRCS =  main.c \
		init.c \
		finish.c \
		utils.c \
		ft_atoi.c

CC = clang

FLAGS = -Wall -Wextra -Werror

INCLUDES = philo.h

OBJS = $(SRCS:.c=.o)

NAME = philo

$(NAME): $(OBJS)
	$(CC) -pthread $(OBJS) -o $(NAME)

%.o : %.c $(INCLUDES)
	$(CC) ${FLAGS} -o $@ -c $<

all : $(NAME)

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY: all clean fclean re
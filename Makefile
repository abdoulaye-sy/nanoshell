NAME = nanoshell
SRC = thing.c
CC = gcc
FLAGS = -Wall -Wextra -Werror

all:
	$(CC) $(FLAGS) $(SRC) -o $(NAME)

clean:
	rm -f $(NAME)

re: clean all

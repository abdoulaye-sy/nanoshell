NAME=nanoshell
SRC=thing.c
#FLAGS=-Wall -Wextra - Werror
FLAGS=-Wall -Wextra

all:
	gcc $(FLAGS) -o $(NAME) $(SRC)

clean:
	rm -f $(NAME)

re: clean all
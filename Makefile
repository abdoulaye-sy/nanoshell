NAME=nanoshell
SRC=thing.c
#FLAGS=-Wall -Wextra - Werror
FLAGS=-Wall -Wextra

all:
	gcc $(FLAGS) -o $(NAME) $(SRC)

foo:
	ls

bar:
	ls -l

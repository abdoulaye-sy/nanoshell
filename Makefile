# Nom du programme final
NAME = nanoshell

# Compilateur utilisé
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -Werror -Iincludes

# Fichiers source
SRCS = src/main.c \
       src/prompt.c \
       src/input.c

# Fichiers objets
OBJS = $(SRCS:.c=.o)

# Règle par défaut
all: $(NAME)

# Compilation du programme
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

# Supprime les fichiers .o
clean:
	rm -f $(OBJS)

# Supprime tout
fclean: clean
	rm -f $(NAME)

# Recompile tout
re: fclean all

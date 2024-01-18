SRC = 	lib_use.c
OBJ = $(SRC:.c=.o)
NAME = lib_use
CFLAGS = -Wall -Wextra -Wpedantic -I../include
CLIBS = ../libpasm.a

all:	$(NAME)

$(NAME): $(OBJ)
	cd .. && $(MAKE) lib
	gcc $(SRC) $(CFLAGS) $(CLIBS) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean
re: $(NAME)

.PHONY : all $(NAME) clean fclean re

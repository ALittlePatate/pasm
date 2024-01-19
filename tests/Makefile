SRC = 	lib_use.c \
		fmemopen.c
OBJ = $(SRC:.c=.o)
NAME = lib_use
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Iinclude -s -Os -fno-ident -fno-asynchronous-unwind-tables
CLIBS = ../build/libpasm.a

all:	$(NAME)

$(NAME): $(OBJ)
	$(MAKE) -C .. lib 
	$(CC) $(OBJ) $(CFLAGS) $(CLIBS) -o $(NAME)

clean:
	@-rm -f $(OBJ)

fclean: clean
	@-rm -f $(NAME)

re: fclean
re:
	$(MAKE) -C .. fclean && $(MAKE) -C .. lib
	$(CC) $(SRC) $(CFLAGS) $(CLIBS) -o $(NAME)

.PHONY : all $(NAME) clean fclean re

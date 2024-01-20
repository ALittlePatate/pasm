SRC = 	src/pasm.c \
	src/file_utils.c \
	src/interpreter_states.c \
	src/instructions.c \
	src/api.c \
	src/debug.c
OBJ = $(SRC:.c=.o)
NAME = pasm
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Iinclude -s -Os -fno-ident -fno-asynchronous-unwind-tables
CLIBS = -lm

all:	$(NAME)

lib: $(OBJ)
	@-mkdir build
	ar rc build/lib$(NAME).a $(OBJ)

$(NAME): fclean
$(NAME): lib
$(NAME): CLIBS += build/lib$(NAME).a
$(NAME):
	$(CC) tests/interpreter.c $(CFLAGS) $(CLIBS) -o build/$(NAME)

interpreter: $(NAME)

clean:
	@-rm -f $(OBJ)
	@-cd tests && $(MAKE) clean

fclean: clean
	@-rm -rf build/
	@-cd tests && $(MAKE) fclean

re: fclean
re: $(NAME)

.PHONY : all $(NAME) clean fclean re interpreter lib

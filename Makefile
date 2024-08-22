SRC = src/pasm.c \
       src/file_utils.c \
       src/interpreter_states.c \
       src/instructions.c \
       src/api.c \
       src/debug.c \
       src/libc.c
OBJ = $(SRC:.c=.o)
NAME = pasm
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Iinclude -s -Os -fno-ident -fno-asynchronous-unwind-tables
CLIBS = -lm

all: $(NAME)

lib: $(OBJ)
	@mkdir -p build
	ar rc build/lib$(NAME).a $(OBJ)

$(NAME): lib
	$(CC) $(CFLAGS) -o build/$(NAME) tests/interpreter.c build/lib$(NAME).a $(CLIBS)

interpreter: $(NAME)

clean:
	@rm -f $(OBJ)
	@cd tests && $(MAKE) clean

fclean: clean
	@rm -rf build/
	@cd tests && $(MAKE) fclean

re: fclean
re: $(NAME)

.PHONY: all $(NAME) clean fclean re interpreter lib
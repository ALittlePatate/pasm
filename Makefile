SRC = 	pasm.c \
	file_utils.c \
	interpreter_states.c \
	instructions.c \
	api.c
OBJ = $(SRC:.c=.o)
NAME = pasm
CFLAGS = -Wall -Wextra -Wpedantic -Iinclude
CLIBS =

all:	$(NAME)

lib: $(OBJ)
	ar rc lib$(NAME).a $(OBJ)

$(NAME): fclean
$(NAME): lib
$(NAME): CLIBS += lib$(NAME).a
$(NAME):
	gcc tests/interpreter.c $(CFLAGS) $(CLIBS) -o $(NAME)

interpreter: $(NAME)

debug: fclean
debug: CFLAGS += -DDEBUG -g3
debug: $(NAME)

clean:
	rm -f $(OBJ)
	cd tests && $(MAKE) clean

fclean: clean
	rm -f $(NAME)
	find . -name "lib$(NAME).a" -delete
	cd tests && $(MAKE) fclean

re: fclean
re: $(NAME)

.PHONY : all $(NAME) clean fclean re interpreter lib debug

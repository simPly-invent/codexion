NAME := codexion
CFLAGS := -Wall -Wextra -Werror -pthread -g
SRCDIR :=coders/file

all:
	$(CC) $(CFLAGS) $(SRCDIR)/*.c -o $(NAME)

clean:

fclean:

re:

.PHONY: all clean fclean clean re
NAME := codexion
CFLAGS := -Wall -Werror -Wextra -pthread -g
SRCDIR :=coders/file

all:
	$(CC) $(CFLAGS) $(SRCDIR)/*.c -o $(NAME)

clean:
	rm codexion
fclean:

re:

.PHONY: all clean fclean clean re
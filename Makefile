NAME := codexion
CFLAGS := -Wall -Wextra -Werror -pthread
SRCDIR :=coders/file
all:
	$(CC) $(CFLAGS) coders/file/*.c -o $(NAME)
$(SRCDIR)/%.o:$(SRCDIR)/%.c
	cc -Wall -Wextra -Werror -Iinclude -c $< -o $@

clean:

fclean:

re:

.PHONY: all clean fclean clean re
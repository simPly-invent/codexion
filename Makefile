NAME := codexion
CFLAGS := -Wall -Wextra -Werror -pthread -g
SRCDIR := coders/file
SRCS := $(SRCDIR)/codexion.c $(SRCDIR)/parser.c $(SRCDIR)/init.c \
		$(SRCDIR)/init_second.c $(SRCDIR)/routine.c $(SRCDIR)/thread.c \
		$(SRCDIR)/thread_monitor.c $(SRCDIR)/dongle_utils.c \
		$(SRCDIR)/dongle_utils_second.c $(SRCDIR)/check_validation.c \
		$(SRCDIR)/check_validation_second.c $(SRCDIR)/utils.c \
		$(SRCDIR)/utils_second.c $(SRCDIR)/main.c $(SRCDIR)/main_second.c
OBJS := $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) *.log *.txt

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
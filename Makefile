SRCS = src/main.c\

OBJS_DIR = obj

OBJS = $(SRCS:src/%.c=$(OBJS_DIR)/%.o)

NAME = minishell

HEADER = includes/minishell.h\
	includes/struct.h

CC = cc

CFLAGS = -Wall -Wextra -Werror -g3

LIBFT = libft/libft.a

valgrind-clean: $(NAME)
	@echo "=== Valgrind without readline leaks ==="
	-valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes --track-origins=yes --suppressions=readline.supp ./$(NAME)

all: $(NAME)

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(NAME): $(LIBFT) $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -L./libft -lft -o $(NAME)

$(OBJS_DIR)/%.o: src/%.c $(HEADER) | $(OBJS_DIR)
	$(CC) -I./libft $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C libft

clean:
	@make clean -C libft
	rm -rf $(OBJS_DIR)

fclean: clean
	@make fclean -C libft
	rm -f $(NAME)

re: fclean all
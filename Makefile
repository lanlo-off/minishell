SRCS = src/main.c\
	src/print_token.c
	src/exec/loop.c\
	src/exec/clean_loop.c
	src/init/init.c\
	src/init/get_env.c
	src/lexer/clean_lexer.c\
	src/print_token.c\
	src/exec/loop.c\
	src/exec/built-in/pwd.c\
	src/exec/built-in/cd.c\
	src/exec/built-in/exit.c\
	src/init/init.c\
	src/lexer/clean_lexer.c\
	src/lexer/lexer_utils.c\
	src/lexer/lexer.c\
	src/lexer/operators.c\
	src/lexer/quotes.c\
	src/lexer/split_sub_utils.c\
	src/lexer/split_subwords.c\
	src/lexer/split_token.c\
	src/parser/clean_parser.c\
	src/parser/expand.c\
	src/parser/expand_utils.c\
	src/parser/parse.c\
	src/parser/token_to_cmd.c


OBJS_DIR = obj

OBJS = $(SRCS:src/%/%.c=$(OBJS_DIR)/%.o)

NAME = minishell

HEADER = includes/minishell.h\
	includes/struct.h

CC = cc

CFLAGS = -Wall -Wextra -Werror -g3

LIBFT = libft/libft.a

#valgrind-clean: $(NAME)
#	@echo "=== Valgrind without readline leaks ==="
#	-valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes --track-origins=yes --suppressions=readline.supp ./$(NAME)

all: $(NAME)

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(NAME): $(LIBFT) $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -L./libft -lft -lreadline -o $(NAME)

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
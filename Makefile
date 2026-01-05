SRCS = src/main.c

SRCS += src/exec/loop.c\
	src/exec/clean_loop.c\
	src/exec/exec.c\
	src/exec/exec_utils.c\
	src/exec/utils.c\
	src/exec/redir.c\
	src/exec/built-in/pwd.c\
	src/exec/built-in/cd.c\
	src/exec/built-in/exit.c\
	src/exec/built-in/env.c\
	src/exec/built-in/echo.c\
	src/exec/built-in/export.c\
	src/exec/built-in/export2.c\
	src/exec/built-in/unset.c\
	src/exec/heredoc.c\
	src/exec/heredoc2.c\
	src/exec/single_cmd_utils.c

SRCS += src/free_error/clean_lexer.c\
	src/free_error/clean_fork.c\
	src/free_error/clean_parser.c\
	src/free_error/clean.c\
	src/free_error/free_lists.c\
	src/free_error/print_error.c

SRCS += src/init/init.c\
	src/init/get_env.c\
	src/init/get_path.c\
	src/init/signals.c

SRCS += src/lexer/lexer.c\
	src/lexer/operators.c\
	src/lexer/quotes.c\
	src/lexer/split_sub_utils.c\
	src/lexer/split_subwords.c\
	src/lexer/split_token.c

SRCS += src/parser/expand.c\
	src/parser/expand_utils.c\
	src/parser/parse.c\
	src/parser/utils.c\
	src/parser/token_to_cmd.c


OBJS_DIR = obj

OBJS = $(SRCS:src/%/%.c=$(OBJS_DIR)/%.o)

NAME = minishell

HEADER = includes/minishell.h\
	includes/struct.h

CC = cc

CFLAGS = -Wall -Wextra -Werror -g3

LIBFT = libft/libft.a

all: $(NAME)

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(NAME): $(LIBFT) $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -L./libft -lft -L/opt/homebrew/opt/readline/lib -lreadline -lhistory -o $(NAME)

$(OBJS_DIR)/%.o: src/%.c $(HEADER) | $(OBJS_DIR)
	@mkdir -p $(dir $@)
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
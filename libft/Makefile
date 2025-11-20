SRCS_A = base/ft_isalpha.c \
	base/ft_isdigit.c \
	base/ft_isalnum.c \
	base/ft_isascii.c \
	base/ft_isprint.c \
	base/ft_strlen.c \
	base/ft_memset.c \
	base/ft_bzero.c \
	base/ft_memcpy.c \
	base/ft_memmove.c \
	base/ft_strlcpy.c \
	base/ft_strlcat.c \
	base/ft_toupper.c \
	base/ft_tolower.c \
	base/ft_strchr.c \
	base/ft_strrchr.c \
	base/ft_strncmp.c \
	base/ft_memcmp.c \
	base/ft_strnstr.c \
	base/ft_atoi.c \
	base/ft_calloc.c \
	base/ft_strdup.c \
	base/ft_substr.c \
	base/ft_strjoin.c \
	base/ft_strtrim.c \
	base/ft_split.c \
	base/ft_itoa.c \
	base/ft_strmapi.c \
	base/ft_striteri.c \
	base/ft_putchar_fd.c \
	base/ft_putstr_fd.c \
	base/ft_putendl_fd.c \
	base/ft_putnbr_fd.c \
	base/ft_memchr.c\
	base/ft_lstnew.c \
	base/ft_lstadd_front.c \
	base/ft_lstsize.c \
	base/ft_lstlast.c \
	base/ft_lstadd_back.c \
	base/ft_lstdelone.c \
	base/ft_lstclear.c \
	base/ft_lstiter.c \
	base/ft_lstmap.c

# ftprintf functions
SRCS_A += ftprintf/ft_printf.c\
	ftprintf/ft_putchar_str.c\
	ftprintf/ft_putnbr_base.c\
	ftprintf/ft_putptr.c

# GNL functions
SRCS_A += gnl/get_next_line.c\
	gnl/get_next_line_utils.c

# Other functions
SRCS_A += other/free_tab_str.c\
	other/ft_strcmp.c

SRC_DIR	= .

OBJS_A = $(SRCS_A:.c=.o)

INC_DIR	= .

NAME = libft.a

HEADER = libft.h\
	ft_printf.h

CC = cc

AR = ar rcs

CFLAGS = -Wall -Wextra -Werror -g

all: $(NAME)

$(NAME): $(OBJS_A) 
	$(AR) $(NAME) $(OBJS_A)

%.o: %.c $(HEADER)
	$(CC) -I. -I./ftprintf $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS_A)

fclean: clean
	rm -f $(NAME)

re: fclean all

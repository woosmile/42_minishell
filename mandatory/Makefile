NAME = minishell
#NAME = ../minishell

CC = cc
CFLAGS = -Werror -Wall -Wextra -g3

MAKE = make
MFLAGS = -C

RM = rm
RMFLAGS = -rf

LIBFT_DIR = ../libft
LIBFT_SRCS = \
../libft/ft_atoi.c \
../libft/ft_bzero.c \
../libft/ft_calloc.c \
../libft/ft_isalnum.c \
../libft/ft_isalpha.c \
../libft/ft_isascii.c \
../libft/ft_isdigit.c \
../libft/ft_isprint.c \
../libft/ft_itoa.c \
../libft/ft_memchr.c \
../libft/ft_memcmp.c \
../libft/ft_memcpy.c \
../libft/ft_memmove.c \
../libft/ft_memset.c \
../libft/ft_putchar_fd.c \
../libft/ft_putendl_fd.c \
../libft/ft_putnbr_fd.c \
../libft/ft_putstr_fd.c \
../libft/ft_split.c \
../libft/ft_strchr.c \
../libft/ft_strdup.c \
../libft/ft_striteri.c \
../libft/ft_strjoin.c \
../libft/ft_strlcat.c \
../libft/ft_strlcpy.c \
../libft/ft_strlen.c \
../libft/ft_strmapi.c \
../libft/ft_strnstr.c \
../libft/ft_strrchr.c \
../libft/ft_strtrim.c \
../libft/ft_substr.c \
../libft/ft_tolower.c \
../libft/ft_toupper.c \
../libft/ft_strncmp.c \
../libft/ft_lstadd_back_bonus.c \
../libft/ft_lstadd_front_bonus.c \
../libft/ft_lstclear_bonus.c \
../libft/ft_lstdelone_bonus.c \
../libft/ft_lstiter_bonus.c \
../libft/ft_lstlast_bonus.c \
../libft/ft_lstmap_bonus.c \
../libft/ft_lstnew_bonus.c \
../libft/ft_lstsize_bonus.c
LIBFT_LIB = libft.a

READLINE_DIR = ../readline

MANDATORY_SRCS = \
minishell.c \
minishell_utils.c \
env_init.c \
env_conv_arr.c \
token_init.c \
token_init_utils.c \
str_split.c \
str_split_utils.c \
cmd_init.c \
cmd_init_utils.c \
expansion.c \
expansion_env_arr.c \
expansion_list_init.c \
expansion_list_utils.c \
builtin.c
MANDATORY_OBJS = $(MANDATORY_SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(MANDATORY_OBJS) $(LIBFT_SRCS)
	$(MAKE) $(MFLAGS) $(LIBFT_DIR) bonus
	$(CC) $(CFLAGS) -o $@ $(MANDATORY_OBJS) \
	-L../libft -lft \
	-L../readline/lib -lreadline -lcurses \
	-L../readline/lib -lhistory

$(MANDATORY_OBJS) : $(MANDATORY_SRCS) 
	$(MAKE) $(MFLAGS) $(READLINE_DIR) all
	$(CC) $(CFLAGS) -c $^

clean :
	$(MAKE) $(MFLAGS) $(LIBFT_DIR) clean
	$(MAKE) $(MFLAGS) $(READLINE_DIR) clean
	$(RM) $(RMFLAGS) $(MANDATORY_OBJS)

fclean : clean
	$(MAKE) $(MFLAGS) $(LIBFT_DIR) fclean
	$(MAKE) $(MFLAGS) $(READLINE_DIR) fclean
	$(RM) $(RMFLAGS) $(NAME)

re : fclean all

.PHONY : all clean fclean re
NAME = minishell

CC = cc
CFLAGS = -Werror -Wall -Wextra -g
LDFLAGS = 	-L./libft -lft \
			-L./readline/lib -lreadline \
			-L./readline/lib -lhistory

MAKE = make
MFLAGS = -C

RM = rm
RMFLAGS = -rf

LIBFT_DIR = ./libft/
LIBFT_LIB = $(LIBFT_DIR)libft.a

READLINE_DIR = ./readline/
READLINE_LIB = $(READLINE_DIR)lib/libreadline.a $(READLINE_DIR)lib/libhistory.a

HEADER = \
./mandatory/minishell.h \
$(LIBFT_DIR)libft.h \
./readline/include/readline/readline.h \
./readline/include/readline/history.h

INCLUDES = -I./mandatory -I./readline/include/readline -I./libft

MANDATORY_SRCS = \
mandatory/builtin.c \
mandatory/builtin2.c \
mandatory/builtin3.c \
mandatory/builtin4.c \
mandatory/builtin5.c \
mandatory/cmd_init_utils.c \
mandatory/cmd_init.c \
mandatory/env_conv_arr.c \
mandatory/env_init.c \
mandatory/expansion_env_arr.c \
mandatory/expansion_str_combine.c \
mandatory/expansion_utils.c \
mandatory/expansion.c \
mandatory/find_cmd_path.c \
mandatory/minishell_utils.c \
mandatory/minishell.c \
mandatory/shell_op.c \
mandatory/shell_op2.c \
mandatory/shell_op3.c \
mandatory/heredoc.c \
mandatory/heredoc2.c \
mandatory/str_split_utils.c \
mandatory/str_split.c \
mandatory/token_init_utils.c \
mandatory/token_init.c \
mandatory/signal.c \
mandatory/unquote.c

MANDATORY_OBJS = $(MANDATORY_SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(LIBFT_LIB) $(READLINE_LIB) $(MANDATORY_OBJS) $(HEADER)
	$(CC) $(CFLAGS) -o $@ $(MANDATORY_OBJS) $(LDFLAGS)

$(LIBFT_LIB) :
	$(MAKE) $(MFLAGS) $(LIBFT_DIR) bonus

$(READLINE_LIB) :
	$(MAKE) $(MFLAGS) $(READLINE_DIR) all

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

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

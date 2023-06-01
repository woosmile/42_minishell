NAME = minishell

MAKE = make
MFLAGS = -C

LIBFT_SRCS = \
libft/ft_atoi.c \
libft/ft_bzero.c \
libft/ft_calloc.c \
libft/ft_isalnum.c \
libft/ft_isalpha.c \
libft/ft_isascii.c \
libft/ft_isdigit.c \
libft/ft_isprint.c \
libft/ft_itoa.c \
libft/ft_memchr.c \
libft/ft_memcmp.c \
libft/ft_memcpy.c \
libft/ft_memmove.c \
libft/ft_memset.c \
libft/ft_putchar_fd.c \
libft/ft_putendl_fd.c \
libft/ft_putnbr_fd.c \
libft/ft_putstr_fd.c \
libft/ft_split.c \
libft/ft_strchr.c \
libft/ft_strdup.c \
libft/ft_striteri.c \
libft/ft_strjoin.c \
libft/ft_strlcat.c \
libft/ft_strlcpy.c \
libft/ft_strlen.c \
libft/ft_strmapi.c \
libft/ft_strnstr.c \
libft/ft_strrchr.c \
libft/ft_strtrim.c \
libft/ft_substr.c \
libft/ft_tolower.c \
libft/ft_toupper.c \
libft/ft_strncmp.c \
libft/ft_lstadd_back_bonus.c \
libft/ft_lstadd_front_bonus.c \
libft/ft_lstclear_bonus.c \
libft/ft_lstdelone_bonus.c \
libft/ft_lstiter_bonus.c \
libft/ft_lstlast_bonus.c \
libft/ft_lstmap_bonus.c \
libft/ft_lstnew_bonus.c \
libft/ft_lstsize_bonus.c

MANDATORY_DIR = mandatory
MANDATORY_SRCS = \
mandatory/minishell.c
MANDATORY_OBJS = $(MANDATORY_SRCS:.c=.o)

#BONUS_DIR = bonus
#BONUS_SRCS = 
#BONUS_OBJS = $(BONUS_SRCS:.c=.o)

ifdef WITH_BONUS
	FINAL_DIR = $(BONUS_DIR)
else
	FINAL_DIR = $(MANDATORY_DIR)
endif

ifdef WITH_BONUS
	FINAL_SRCS = $(BONUS_SRCS)
else
	FINAL_SRCS = $(MANDATORY_SRCS)
endif

ifdef WITH_BONUS
	FINAL_OBJS = $(BONUS_OBJS)
else
	FINAL_OBJS = $(MANDATORY_OBJS)
endif

all : $(NAME)
bonus : 
	make WITH_BONUS=1 all

$(NAME) : $(FINAL_OBJS) $(FINAL_SRCS) $(LIBFT_SRCS)

$(FINAL_OBJS) : $(FINAL_SRCS)
	$(MAKE) $(MFLAGS) $(FINAL_DIR) all

clean :
	$(MAKE) $(MFLAGS) $(MANDATORY_DIR) clean
#	$(MAKE) $(MFLAGS) $(BONUS_DIR) clean


fclean : clean
	$(MAKE) $(MFLAGS) $(MANDATORY_DIR) fclean
#	$(MAKE) $(MFLAGS) $(BONUS_DIR) fclean
	$(RM) $(RMFLAGS) $(NAME)

re : fclean all

.PHONY : all bonus clean fclean re
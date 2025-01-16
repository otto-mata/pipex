NAME=pipex
BONUS=$(NAME)_bonus
CC=clang
CFLAGS=-Wall -Werror -Wextra

SRC=./mandatory/mandatory.c \
	./mandatory/utils.c

OBJS=$(SRC:.c=.o)

BSRC=./bonus/heredoc.c \
	./bonus/main.c \
	./bonus/pipe_utils.c \
	./bonus/pipeline.c \
	./bonus/utils0.c \
	./bonus/utils1.c

BOBJS=$(BSRC:.c=.o)

$(NAME): $(OBJS)
	@make -C libft
	$(CC) $(CFLAGS) $^ -Llibft -lft -o $@

$(BONUS): $(BOBJS)
	@make -C libft
	$(CC) $(CFLAGS) $^ -Llibft -lft -o $@

bonus: $(BONUS)

clean:
	rm -f $(OBJS)
	rm -f $(BOBJS)
	@make clean -C libft

fclean: clean
	rm -f $(NAME)
	rm -f $(BONUS)
	@make fclean -C libft

re: fclean all

all: $(NAME)

default: all

.PHONY: all re clean fclean bonus
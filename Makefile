SRCS			= push_swap.c parse_input.c sort_control.c stack_setup.c llist_utils.c utils.c sorting_tools.c sorting_algorithms/ascending_index.c \
				  sorting_algorithms/ascending_index_utils.c sorting_algorithms/chunk_sort.c

OBJS			= $(SRCS:.c=.o)

CC				= gcc
RM				= rm -f
CFLAGS			= -Wall -Wextra -Werror -I. 

NAME			= push_swap

all:			$(NAME)

$(NAME):		$(OBJS)
				$(CC) $(CFLAGS) -o $(NAME) $(OBJS) 

bonus:			all

clean:
				$(RM) $(OBJS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

.PHONY:			all clean fclean re bonus
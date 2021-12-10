#!/bin/bash
gcc -Wall -Wextra -Werror -I. -L/Users/jgoad/Projects/Cursus/push_swap/libft -lft -o push_swap push_swap.c parse_input.c llist_utils.c utils.c sorting_tools.c stack_setup.c sort_control.c ./sorting_algorithms/ascending_index.c ./sorting_algorithms/ascending_index_utils.c ./sorting_algorithms/chunk_sort.c
./push_swap 9 40 39 44 41 49 22 1 42 13 15 35 7 16 43 25 37 32 27 38 28 4 17 6

#9 40 39 44 41 49 22 1 42 13 15 35 7 16 43 25 37 32 27 38 28 4 17 6 - set of 24 random
#"9", "40", "39", "44", "41", "49", "22", "1", "42", "13", "15", "35", "7", "16", "43", "25", "37", "32", "27", "38", "28", "4", "17", "6"
#7 1 3 6 2 0 5 4 - set of 8 index matched
#"5", "0", "3", "1", "7", "2", "6", "4"
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_push_swap.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:18:57 by jgoad             #+#    #+#             */
/*   Updated: 2021/12/10 17:05:38 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	FT_PUSH_SWAP_H
# define FT_PUSH_SWAP_H

//System includes
# include <limits.h>
# include <stdlib.h>

//Make sure to remove
# include <stdio.h>

//Libft
# include "libft/libft.h"

//Linked list struct
typedef struct s_stack
{
	int				value;
	int				index;
	struct s_stack	*previous;
	struct s_stack	*next;
}				t_stack;


//Vars struct for ascending order sort
//Struct to hold variables
typedef struct s_vars
{
	//Vars for start_index function
	int	count;
	int	count_tmp;
	int inner_ctl;
	int outer_ctl;

	//General vars for ascending index algorithm
	int start_index;
	int current_index;
	int stack_size;
	int	index_limit;

	//Vars for asc_find_closest
	int distance;
	int count_a;
	int count_b;
	int target_index_a;
	int target_index_b;
	int stack_size_a;
	int stack_size_b;
	int distance_a;
	int distance_b;
}	t_vars;

typedef struct s_vars2
{
	//Track sizes for total stack and each chunk
	int	stack_size_total;
	int	stack_size_current;
	int chunk_size[9];
	int chunk_start[9];
	int	chunk_end[9];

	//Track indices for each chunk
	//Can also be stored in an array

}	t_vars2;

//Main functions
void	control(t_stack **a, t_stack **b);
void	sort_control(t_stack **a, t_stack **b);


//Parse functions
int	parse_integer(char *str, int *n);
int	parse_duplicate(t_stack* a);

//Setup functions
void	setup_control(t_stack **a, t_stack **b);
t_stack	*clone_stack(t_stack *src, t_stack **dst);
t_stack *find_index(t_stack *sorted);
t_stack *apply_index(t_stack *dst, t_stack *src);

//Ascending index sort functions
void	ascending_index(t_stack **a, t_stack **b, int **actions);
int		find_start(t_stack *a);
void	asc_push_unsorted(t_stack **a, t_stack **b, t_vars **vars, int **actions);
void	asc_push_sorted_greater(t_stack **a, t_stack **b, t_vars **vars, int **actions);
void	asc_push_sorted_lesser(t_stack **a, t_stack **b, t_vars **vars, int **actions);
void	asc_push_match(t_stack **a, t_stack **b, t_vars **vars, int **actions);

//Utils for ascending index sort
t_vars *init_vars(t_vars *vars, t_stack *a);
t_vars *start_reset_vars(t_vars *vars, t_stack *a);
int		find_start_freevars(t_vars **vars);
int		find_lowest(t_stack *stack);
int		find_highest(t_stack *stack);
int		asc_find_rotate_direction(t_stack *stack, int index);
void	asc_find_closest(t_stack *a, t_stack *b, t_vars **vars);



//Chunk sort functions
void	chunk_sort(t_stack **a, t_stack **b, int **actions);
t_vars2	*chunk_init_vars(t_stack **a, t_vars2 *vars);
void	create_chunks(t_stack **a, t_stack **b, int **actions, t_vars2 *vars);
int		check_sort_chunks(t_stack **stack, int direction);
void	swap_chunks(t_stack **a, t_stack **b, int **actions, t_vars2 *vars);
void	final_sort_chunks(t_stack **a, t_stack **b, int **actions, t_vars2 *vars);
int		find_start_chunk(t_stack **stack, t_vars2 *vars, int chunk);

//Utility functions
int	write_err(void);
long	ft_atol(const char *str);
t_stack	*stack_sort(t_stack *solved);

//Linked list functions
void	stackadd_back(t_stack **lst, t_stack *new);
void	stackadd_front(t_stack **lst, t_stack *new);
t_stack	*stacknew(int value, int index, t_stack *previous);
int		stacksize(t_stack *lst);
t_stack	*stacklast(t_stack *lst);
void	stackclear(t_stack **lst);
void	stackdelone(t_stack *lst);
void	stackdeltop(t_stack **lst);

//Sorting actions
t_stack	*swap_a(t_stack *a, int **actions);
t_stack	*swap_b(t_stack *b, int **actions);
void	swap_both(t_stack **a, t_stack **b, int **actions);
void	push_a(t_stack **a, t_stack **b, int **actions);
void	push_b(t_stack **b, t_stack **a, int **actions);
t_stack	*rotate_a(t_stack *a, int **actions);
t_stack	*rotate_b(t_stack *b,int **actions);
void	rotate_both(t_stack **a, t_stack **b,int **actions);
t_stack	*rev_rotate_a(t_stack *a, int **actions);
t_stack	*rev_rotate_b(t_stack *b,int **actions);
void	rev_rotate_both(t_stack **a, t_stack **b,int **actions);


#endif
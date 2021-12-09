/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ascending_index.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 12:18:35 by jgoad             #+#    #+#             */
/*   Updated: 2021/12/02 15:51:28 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_push_swap.h"

//This file contains the functions required to run ascending index sort algorithm

//Control function for ascending index sorting algorithm
void	ascending_index(t_stack **a, t_stack **b, int **actions)
{
	t_vars *vars;

	//Initialize variables
	vars = NULL;
	vars = init_vars(vars, *a);

	//Push unsorted values to stack b
	asc_push_unsorted(a, b, &vars, actions);
	
	//Push values back to stack a until stack b is empty
	while(*b)
	{
		//Find the next closest match to push, based on current stack positions
		asc_find_closest(*a, *b, &vars);

		//Push from b to a based on match found
		asc_push_match(a, b, &vars, actions);
	}

	//Rotate a back to index 0 on top
	if (asc_find_rotate_direction(*a, 0) == -1)
	{
		while ((*a)->index != 0)
			*a = rev_rotate_a(*a, actions);
	}
	else
	{
		while ((*a)->index != 0)
			*a = rotate_a(*a, actions);
	}
}

//Function to rotate stacks until there is a match on top of each
void	asc_push_match(t_stack **a, t_stack **b, t_vars **vars, int **actions)
{
	//Rotate stacks until target index is on top of each stack
	while (((*a)->index != (*vars)->target_index_a) || ((*b)->index != (*vars)->target_index_b))
	{
		//Check if we can do a double rotate action
		if ((asc_find_rotate_direction(*a, (*vars)->target_index_a) == 1) && (asc_find_rotate_direction(*b, (*vars)->target_index_b) == 1))
			rotate_both(a, b, actions);
		//Check if we can do a double rev rotate action
		else if ((asc_find_rotate_direction(*a, (*vars)->target_index_a) == -1) && (asc_find_rotate_direction(*b, (*vars)->target_index_b) == -1))
			rev_rotate_both(a, b, actions);
		//Otherwise run single actions
		else
		{
			//Run action for a
			if (asc_find_rotate_direction(*a, (*vars)->target_index_a) == -1)
				*a = rev_rotate_a(*a, actions);
			else if (asc_find_rotate_direction(*a, (*vars)->target_index_a) == 1)
				*a = rotate_a(*a, actions);
			//run action for b
			if (asc_find_rotate_direction(*b, (*vars)->target_index_b) == -1)
				*b = rev_rotate_b(*b, actions);
			else if (asc_find_rotate_direction(*b, (*vars)->target_index_b) == 1)
				*b = rotate_b(*b, actions);
		}
	}
	//Push from b to a once match is in position
	if ((*b)->index < (*a)->index)
		push_a(a, b, actions);
	//If b->index is larger than top index of a, rotate a once before pushing
	else if ((*b)->index > (*a)->index)
	{
		*a = rotate_a(*a, actions);
		push_a(a, b, actions);
	}
}

//Find the integer with the most integers that follow that are consecutively larger.
int	find_start(t_stack *a)
{
	//Declare and intialize vars struct
	t_vars *vars;

	vars = NULL;
	vars = init_vars(vars, a);
	//Run rotate until we have completed one complete loop of the stack
	while (vars->outer_ctl < vars->stack_size)
	{
		//Reset vars before inner loop
		vars = start_reset_vars(vars, a);
		//Run rotate until we have done one complete loop of the stack
		while (vars->inner_ctl < vars->stack_size)
		{
			//If index of current element is higher than previous index, increment count and set new highest index
			if (a->index > vars->current_index)
			{
				vars->count_tmp++;
				vars->current_index = a->index;
			}
			//Rotate once to next node
			a = rotate_a(a, NULL);
			vars->inner_ctl++;
		}
		//If count from inner loop is higher than previous count, set new highest count and new start index
		if (vars->count_tmp > vars->count)
		{
			vars->count = vars->count_tmp;
			vars->start_index = a->index;
		}
		//Rotate once to test next start position
		a = rotate_a(a, NULL);
		vars->outer_ctl++;
	}
	//Free vars struct and return the starting index
	return (find_start_freevars(&vars));
}

//Push unsorted integers to stack b
void	asc_push_unsorted(t_stack **a, t_stack **b, t_vars **vars, int **actions)
{
	int i;

	i = 0;
	//Find the starting value that allows the most values to be kept in stack a
	(*vars)->start_index = find_start(*a);

	//Rotate a until the start index is found (need to check which direction is fastest)
	if (asc_find_rotate_direction(*a, (*vars)->start_index) == 1)
	{
		while ((*a)->index != (*vars)->start_index)
			*a = rotate_a(*a, actions);
	}
	else
	{
		while ((*a)->index != (*vars)->start_index)
			*a = rev_rotate_a(*a, actions);
	}

	//Push values to stack b
	//THIS IS RUNNING EVEN IF STACK IS ALREADY SORTED, FIX THIS
	(*vars)->current_index = (*vars)->start_index;
	while(i < (*vars)->stack_size)
	{
		//If index of current node is greater than current highest index, update current_index and rotate stack a
		if ((*a)->index > (*vars)->current_index)
		{
			(*vars)->current_index = (*a)->index;
			*a = rotate_a(*a, actions);
		}
		//If index of current node is less than current higheest index, push the node to stack b. No need to rotate stack a in this case, as we have moved to the next node by deleting the top noded
		else if ((*a)->index < (*vars)->current_index)
			push_b(b, a, actions);
		//If no conditions are met (only the first iteration) rotate a 
		else
			*a = rotate_a(*a, actions);
		i++;
	}
}

//Push integers back to stack a in the correct position
void	asc_push_sorted_greater(t_stack **a, t_stack **b, t_vars **vars, int **actions)
{
	t_stack *next_node;


	//If b->index is greater than a->index, rotate a
	while (*b && (*b)->index > (*a)->index)
	{
		//Find current highest index in stack a
		(*vars)->index_limit = find_highest(*a); 
		//Set next_node to the value of the next node in stack a
		next_node = (*a)->next;
		//Check if b fits between the current node and the next node
		//If it does, rotate once and then push
		if ((*b)->index < next_node->index)
		{
			*a = rotate_a(*a, actions);
			push_a(a, b, actions);
		}
		//If index of b is higher than all of the indices in stack a, we need to place it below the highest current index in stack a
		else if ((*a)->index == (*vars)->index_limit)
		{
			*a = rotate_a(*a, actions);
			push_a(a, b, actions);
		}
		//If b does not fit rotate
		else
			*a = rotate_a(*a, actions);
	}
}

//Push integers back to stack a in the correct position
void	asc_push_sorted_lesser(t_stack **a, t_stack **b, t_vars **vars, int **actions)
{
	t_stack *next_node;

	//If b->index is less than a->index reverse rotate a
	while (*b && (*b)->index < (*a)->index)
	{
		//Find current lowest index in stack a
		(*vars)->index_limit = find_lowest(*a); 

		//Set next_done to the value of the previous node in stack a (which is the last node in the current stack)
		next_node = stacklast(*a);
		
		//Check if b fits between current node and the previous node
		if ((*b)->index > next_node->index)
			push_a(a,b, actions);
		//If index of b is lower than all of the indices in stack a, we need to place it above the lowest current index in stack a
		else if ((*a)->index == (*vars)->index_limit)
			push_a(a, b, actions);
		else
			*a = rev_rotate_a(*a, actions);
	}
}

//Function to find the closest integer to move based on relative positions of stack a and stack b
void	asc_find_closest(t_stack *a, t_stack *b, t_vars **vars)
{
	(*vars)->stack_size_a = stacksize(a);
	(*vars)->stack_size_b = stacksize(b);
	(*vars)->count_b = 0;
	(*vars)->distance = 100000;
	//Rotate completely through b
	while ((*vars)->count_b < (*vars)->stack_size_b)
	{
		//Reset count for stack a before starting a new loop
		(*vars)->count_a = 0;
		(*vars)->distance_b = (*vars)->count_b;
		//Rotate completely through stack a
		while ((*vars)->count_a < (*vars)->stack_size_a)
		{
			(*vars)->distance_a = (*vars)->count_a;
			//If there is an index match AND this match has a shorter distance, update target indexes and distance
			if ((a->index == b->index + 1) || (a->index == b->index - 1))
			{
				//Get absolute distance of index from top of each stack
				if ((*vars)->count_a > ((*vars)->stack_size_a / 2))
					(*vars)->distance_a = (*vars)->stack_size_a - (*vars)->count_a;
				if ((*vars)->count_b > ((*vars)->stack_size_b / 2))
					(*vars)->distance_b = (*vars)->stack_size_b - (*vars)->count_b;
				//If distance is shorter, update distance and target indexes
				if (((*vars)->distance_b + (*vars)->distance_a) < (*vars)->distance)
				{
					(*vars)->distance = (*vars)->distance_a + (*vars)->distance_b;
					(*vars)->target_index_a = a->index;
					(*vars)->target_index_b = b->index;
				}
			}
			//Rotate stack a and increment count
			a = rotate_a(a, NULL);
			(*vars)->count_a++;
		}
		//Rotate b once and then run through a again
		b = rotate_b(b, NULL);
		(*vars)->count_b++;
	}
}
//If we want to use double rotations we need to know which direction we want to rotate each stack






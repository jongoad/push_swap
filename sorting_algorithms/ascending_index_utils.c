/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ascending_index_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 14:06:25 by jgoad             #+#    #+#             */
/*   Updated: 2021/12/10 15:13:56 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_push_swap.h"

//This file includes utility functions for ascending index sorting alogrithm

//Function to initialize vars struct for find_start
t_vars *init_vars(t_vars *vars, t_stack *a)
{
	//Allocate and initialize vars struct
	vars = (t_vars *)malloc(sizeof(t_vars));

	//Initialize vars for start_index function
	vars->count = 1;
	vars->count_tmp = 1;
	vars->inner_ctl = 0;
	vars->outer_ctl = 0;

	//Initialize general vars
	vars->start_index = 0;
	vars->current_index = 0;
	vars->stack_size = stacksize(a);
	vars->index_limit = 0;

	//Return struct
	return (vars);
}

//Function to reset vars before inner loop for find_start
t_vars *start_reset_vars(t_vars *vars, t_stack *a)
{
	//Reset vars
	vars->count_tmp = 1; //Count always starts at one, because minimum set size is one element
	vars->current_index = a->index;
	vars->inner_ctl = 0;

	//Return struct
	return (vars);
}

//Function to free vars struct and return start index
int	find_start_freevars(t_vars **vars)
{
	int start_index;

	//Transfer start_index to stack from heap location
	start_index = (*vars)->start_index;

	//Free struct
	free(*vars);
	vars = NULL;

	//Return start_index
	return (start_index);
}

//Function to find the lowest index currently in a stack
int	find_lowest(t_stack *stack)
{
	int index;

	index = stack->index;
	while (stack)
	{
		if (stack->index < index)
			index = stack->index;
		stack = stack->next;
	}
	return (index);
}

//Function to find the lowest index currently in a stack
int	find_highest(t_stack *stack)
{
	int index;

	index = stack->index;
	while (stack)
	{
		if (stack->index > index)
			index = stack->index;
		stack = stack->next;
	}
	return (index);
}

//Function to find the best direction to rotate a stack to get to the desired index
int	asc_find_rotate_direction(t_stack *stack, int index)
{
	int count;
	int	count_tmp;
	t_stack *ptr_tmp;

	count = 0;
	count_tmp = 0;
	
	//If we are already on the index return 0
	if (stack->index == index)
		return (0);
	ptr_tmp = NULL;
	//Clone stack and apply index to prevent actual stack from being manipulated
	ptr_tmp = clone_stack(stack, &ptr_tmp);
	ptr_tmp = apply_index(ptr_tmp, stack);
	//Find action count using rotate
	while (ptr_tmp->index != index)
	{
		ptr_tmp = rotate_a(ptr_tmp, NULL);
		count_tmp++;
	}
	//Need to completely reset the cloned stack
	stackclear(&ptr_tmp);
	ptr_tmp = clone_stack(stack, &ptr_tmp);
	ptr_tmp = apply_index(ptr_tmp, stack);
	count = count_tmp;
	count_tmp = 0;
	//Find action count using reverse rotate
	while (ptr_tmp->index != index)
	{
		ptr_tmp = rev_rotate_a(ptr_tmp, NULL);
		count_tmp++;
	}
	//Return -1 if reverse rotate is faster, or 1 if rotate is faster
	if (count_tmp < count)
	{
		stackclear(&ptr_tmp);
		return (-1);
	}
	else
	{
		stackclear(&ptr_tmp);
		return (1);
	}
}
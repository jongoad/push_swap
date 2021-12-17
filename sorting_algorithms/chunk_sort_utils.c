/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_sort_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 15:08:09 by jgoad             #+#    #+#             */
/*   Updated: 2021/12/14 18:48:20 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_push_swap.h"

//This file contains utility functions needed for the chunk sorting algorithm

//Function to get the largest index in the current stack
int get_largest_index(t_stack *stack)
{
    int stack_size;
    int largest;
    int tmp;

    tmp = stack->index;
    stack_size = stacksize(stack);
    largest = stack->index;
    while (stack_size > 0)
    {
        stack = rotate_a(stack, NULL);
        if (stack->index > largest)
            largest = stack->index;
        stack_size--;
    }
    while (stack->index != tmp)
        stack = rotate_a(stack, NULL);
    return (largest);
}

//Function to get the largest index in the current stack
//Rewrite these to not use rotate
int get_smallest_index(t_stack *stack)
{
    int stack_size;
    int smallest;
    int tmp;

    tmp = stack->index;
    stack_size = stacksize(stack);
    smallest = stack->index;
    while (stack_size > 0)
    {
        stack = rotate_a(stack, NULL);
        if (stack->index < smallest)
            smallest = stack->index;
        stack_size--;
    }
    while (stack->index != tmp)
        stack = rotate_a(stack, NULL);
    return (smallest);
}

//Function to count number of indices in a chunk less the number of keep_indices in that chunk
int get_chunk_count(int chunk, t_vars3 *vars)
{
    int count;
    int i;
    
    count = vars->chunk_size[chunk];
    i = 0;
    //Iterate through keep_indices and decrement count if and values are found that belong to current chunk
    while (vars->keep_indices[i] != -1)
    {
        if ((vars->keep_indices[i] >= vars->chunk_start[chunk]) && (vars->keep_indices[i] <= vars->chunk_end[chunk]))
            count--;
        i++;
    }
    return (count);
}

//Function to check if current index is in the keep_indices array 
int check_keep(int index, t_vars3 *vars)
{
    int i;

    i = 0;
    while (vars->keep_indices[i] != -1)
    {
        //Return 1 if the index passed is matched in the keep_indices array
        if (index == vars->keep_indices[i])
            return (1);
        i++;
    }
    //Return 0 if no match found
    return (0);
}

//Function to initialize vars struct
t_vars3 *init_vars_3(t_stack **a, t_vars3 *vars)
{
    int i;

    i = 0;
    vars = (t_vars3*)malloc(sizeof(t_vars3));

    //Init stack size
    vars->stack_size_a = stacksize(*a);
    //Init chunk_sizes and index values
    while (i < 20)
    {
        vars->chunk_size[i] = 0;
        vars->chunk_start[i] = -1;//Init to -1 because 0 is a valid index
        vars->chunk_end[i] = -1; //Init to -1 for consistency
        i++;
    }
    //Init keep_count to 0;
    vars->keep_count = 0;
    
    //Init keep indices to -1
    i = 0;
    while (i < 500)
    {
        vars->keep_indices[i] = -1;
        i++;
    }
    return (vars);
}

//Function to initialize chunk information
t_vars3 *init_chunk_info(t_vars3 *vars, int chunk_count)
{
    int i;
    int size_tot;

    i = 0;
    size_tot = 0;
    vars->chunk_count = chunk_count; //Copy chunk count to struct
  	//Assign chunk sizes based on stack size
	while (i < chunk_count)
	{
		vars->chunk_size[i] = (vars->stack_size_a / chunk_count);
		i++;
	}
	//Add remainder (if any) to the final chunk.
	vars->chunk_size[chunk_count - 1] = vars->chunk_size[chunk_count - 1] + (vars->stack_size_a % chunk_count);

    //Use chunk sizes to assign start and end index values for each chunk.
	i = 0;
	while (i < chunk_count)
	{
		//Start index is equal to previous total size
		vars->chunk_start[i] = size_tot;
		//Set new running total
		size_tot = size_tot + vars->chunk_size[i];
		//End index is equal to running total of all chunk sizes minus one
		vars->chunk_end[i] = size_tot - 1;
		i++;
	}
	return (vars);
}

//Function to find the best direction to rotate a stack to get to the desired index
int	chunk_find_rotate_direction(t_stack *stack, int index)
{
    int count;
    int stack_size;
    t_stack *tmp;

    stack_size = stacksize(stack);
    tmp = stack;
    count = 0;
    if (stack->index == index)
        return (0); //Return 0 if no rotation is required
    //Iterate through stack until the target index is found
    while (tmp)
    {
        count++;
        if (tmp->index == index)
            break;
        tmp = tmp->next;
    }
    //If count is past midpoint return -1 for reverse rotate
    if (count >= (stack_size / 2))
        return (-1);
    //Else return 1 for rotate
    else
        return (1);
}

//Function to rotate stack to proper top index based on most efficient direction
void    chunk_rotate_to_target(t_stack **stack, int target_index, int **actions, char c)
{
    if (chunk_find_rotate_direction(*stack, target_index) == -1)
	{
        if (c == 'a')
        {
		    while ((*stack)->index != target_index)
			    *stack = rev_rotate_a(*stack, actions);
        }
        else if (c == 'b')
        {
            while ((*stack)->index != target_index)
			    *stack = rev_rotate_b(*stack, actions);
        }

	}
	else
	{
		if (c == 'a')
        {
		    while ((*stack)->index != target_index)
			    *stack = rotate_a(*stack, actions);
        }
        else if (c == 'b')
        {
            while ((*stack)->index != target_index)
			    *stack = rotate_b(*stack, actions);
        }
	}
} 

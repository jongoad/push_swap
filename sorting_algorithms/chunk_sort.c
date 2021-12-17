/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 13:54:33 by jgoad             #+#    #+#             */
/*   Updated: 2021/12/13 16:55:31 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

//Algorithm to sort using chunks


void	chunk_sort(t_stack **a, t_stack **b, int **actions)
{
	t_vars2 *vars;
	//int	swap_count; //Variable to control the number of times swap function is run
	//swap_count = 2;
	//Initialize vars struct
	vars = NULL;
	vars = chunk_init_vars(a, vars);

	//Split raw stack input into quasi-sorted chunks
	create_chunks(a, b, actions, vars);

	//Apply swap until sorted
	
	//while (!check_sort_chunks(a, 1) || !check_sort_chunks(b, -1))
	//swap_chunks(a, b, actions, vars);
	/*
	//Push from b to a until b is empty
	while (*b)
		push_a(a, b, actions);
	*/
	//Apply final sort
	final_sort_chunks(a, b, actions, vars);
}

//Function to initialize vars struct
//THIS WILL BREAK FOR ANY STACKS UNDER SIZE 8!!!!!!
//Rotations are pushing values over the top of the stack and back to the bottom, messing up order. Maybe need to rotate back afterwards (JANKY FIXED, LOOK HARDER). ALSO STACKS NOT EVEN AT THE END WITH EVEN INPUTS
t_vars2	*chunk_init_vars(t_stack **a, t_vars2 *vars)
{
	int i;
	int size_tot;
	vars = malloc(sizeof(t_vars2));

	i = 0;
	size_tot = 0;
	//Initialize stack size
	vars->stack_size_total = stacksize(*a);

	//Initialize 9th element of chunk arrays to 0 for loop control
	//Will need to switch index to start at 1 to make this work cleanly
	//Maybe jsut keep start index as 0 and use -1
	//(*vars->chunk_size[8]) = 0;
	//(*vars->chunk_start[8]) = -1;
	//(*vars->chunk_end[8]) = -1;

	//Assign chunk sizes based on stack size
	while (i < 8)
	{
		vars->chunk_size[i] = (vars->stack_size_total / 8);
		i++;
	}
	//Add remainder (if any) to the final chunk. (Do I want to split the remainder between two chunks?, does it matter how even chunk size is?)
	vars->chunk_size[7] = vars->chunk_size[7] + (vars->stack_size_total % 8);

	//Use chunk sizes to assign start and end index values for each chunk.
	i = 0;
	while (i < 8)
	{
		//Start index is equal to previous total size
		vars->chunk_start[i] = size_tot;
		//Set new running total
		size_tot = size_tot + vars->chunk_size[i];
		//End index is equal to running total of all chunk sizes
		vars->chunk_end[i] = size_tot - 1;
		i++;
	}
	return (vars);
}

//Function to control the creation of chunks
void	create_chunks(t_stack **a, t_stack **b, int **actions, t_vars2 *vars)
{
	int i;
	int push_count;
	int rotate_count;

	push_count = 0;
	rotate_count = 0;

	//1. Push the smallest 50% of values to stack b.
	vars->stack_size_current = stacksize(*a);
	i = 0;
	while (i < vars->stack_size_current)
	{
		//Push all of chunks 0, 1, 2, and 3
		if ((*a)->index <= vars->chunk_end[3])
			push_b(b, a, actions);
		//Else rotate to next position
		else
			*a = rotate_a(*a, actions);
		i++;
	}

	//2. Push the smallest 50% of values to stack b.
	vars->stack_size_current = stacksize(*a);
	i = 0;
	while (i < vars->stack_size_current)
	{
		//Push all of chunks 4 and 5
		if ((*a)->index <= vars->chunk_end[5])
			push_b(b, a, actions);
		//Else rotate to next position
		else
			*a = rotate_a(*a, actions);
		i++;
	}
	//3. Push the smallest 50% of values to stack b. Only chunk 7 is left in stack a
	vars->stack_size_current = stacksize(*a);
	i = 0;
	while (i < vars->stack_size_current)
	{
		//Push all of chunk 6
		if ((*a)->index <= vars->chunk_end[6])
			push_b(b, a, actions);
		//Else rotate to next position
		else
			*a = rotate_a(*a, actions);
		i++;
	}
	//4. Push the top chunk of stack b back to stack a. The whole chunk is at the top so just push chunk_size[6] times.
	i = vars->chunk_size[6];
	while (i > 0)
	{
		push_a(a, b, actions);
		i--;
	}
	//5. Push the largest 50% of values of top 2 chunks of stack b to stack a. (chunk 5)
	//Need to reverse rotate back until all elements of chunk 4 are on top of stack after pushing chunk 5
	i = vars->chunk_size[4] + vars->chunk_size[5]; //Need to iterate through both chunks to seperate (BECAUSE WE ARE NOT ROTATIN THROUGH ENTIRE STACK, POSITIONING CAN BE SCRWEWED UP! SEE COMMENT ABOVE) 
	//Also this is inefficient if we get lucky push the whole stack right away it will keep rotating through other stack. Ez fix is to reverse rotate by size of other stack, but not efficient.
	//If I add a push count, I can tell if there are any elements left to push, and stop rotating, then reverse rotate back less times.
	while (i > 0)
	{
		//Push all of chunk 5
		if ((*b)->index >= vars->chunk_start[5] && (*b)->index <= vars->chunk_end[5])
		{
			push_a(a, b, actions);
			push_count++;
		}
		//Else rotate to next position
		else if (push_count == vars->chunk_size[5])
			break;
		else
		{
			*b = rotate_b(*b, actions);
			rotate_count++;
		}
		i--;
	}
	while (rotate_count > 0)
	{
		*b = rev_rotate_b(*b, actions);
		rotate_count--;
	}



	//6. Push the top chunk of stack b to stack a. (chunk 4)
		//- Now there are 4 chunks containing 50% of total values in the correct chunk order in stack a.
	i = vars->chunk_size[4];
	while (i > 0)
	{
		push_a(a, b, actions);
		i--;
	}
	//7. Push the largest 50% of values from the remaining chunks in stack b to stack a. (push chunks 3 and 2)
	vars->stack_size_current = stacksize(*b);
	i = 0;
	while (i < vars->stack_size_current)
	{
		//Push all of chunks 3 and 2
		if ((*b)->index <= vars->chunk_end[3] && (*b)->index >= vars->chunk_start[2])
			push_a(a, b, actions);
		//Else rotate to next position
		else
			*b = rotate_b(*b, actions);
		i++;
	}
	//8. Push the largest 50% of values from the remaining chunk in stack b to stack a. (chunk 1)
	vars->stack_size_current = stacksize(*b);
	i = 0;
	while (i < vars->stack_size_current)
	{
		//Push all of chunks 3 and 2
		if ((*b)->index >= vars->chunk_start[1])
			push_a(a, b, actions);
		//Else rotate to next position
		else
			*b = rotate_b(*b, actions);
		i++;
	}
	//9. Push the top chunk from stack a to stack b. (chunk 1)
	i = vars->chunk_size[1];
	while (i > 0)
	{
		push_b(b, a, actions);
		i--;
	}
	//10. Push the smallest 50% of values from the top chunk in stack a to stack b. (chunk 2)
	i = vars->chunk_size[2] + vars->chunk_size[3]; //Check notation on this line. Need to iterate through both chunks to seperate
	rotate_count = 0;
	push_count = 0;
	while (i > 0)
	{
		//Push all of chunk 2
		if ((*a)->index <= vars->chunk_end[2] && (*a)->index >= vars->chunk_start[2])
		{
			push_b(b, a, actions);
			push_count++;
		}
		//Else rotate to next position
		else if (push_count == vars->chunk_size[2])
			break;
		else
		{
			*a = rotate_a(*a, actions);
			rotate_count++;
		}
		i--;
	}
	while (rotate_count > 0)
	{
		*a = rev_rotate_a(*a, actions);
		rotate_count--;
	}
	//11. Push the top chunk from stack a to stack b. (chunk 3)
		//- Now there are 8 equal chunks, with 4 in each stack. All chunks are in the correct relative position.
	i = vars->chunk_size[3];
	while (i > 0)
	{
		push_b(b, a, actions);
		i--;
	}


	//For stack size divisible by 8, we can easily split evenly
	//For odd stack size and not divisible by 8 we need to track individual chunks sizes
		//When splitting a chunk, the chunk being left behind will always contain more elements if necessary 
}

//Function to apply swaps to increase level of sortedness
//Going to need to use vars struct for this function for sure
void	swap_chunks(t_stack **a, t_stack **b, int **actions, t_vars2 *vars)
{
	int	rotate_count_a;
	int rotate_count_b;
	int stack_size_a;
	int	stack_size_b;
	t_stack *tmp_a;
	t_stack *tmp_b;

	rotate_count_a = 1;//Need to perform actions one less time than the stack size so the first and last elements are never compared
	rotate_count_b = 1;
	stack_size_a = stacksize(*a);
	stack_size_b = stacksize(*b);
	//Iterate through both stacks attempting to swap elements in order
	//For stack a, we want to check for ascending order, and for stack b we want to check for descending order
	//Attemp to use double swap and double rotate when possible

	//Loop until both stacks have done one full rotation
	//Need to apply a control so no action is performed if a full rotation has already been completed?? This may be unecessary
	//When values rotate over the comparison is wrong (comparing end of stack to start of stack)
	while (rotate_count_a < stack_size_a && rotate_count_b < stack_size_b)
	{
		//Set tmp values to next element for each stack
		tmp_a = (*a)->next;
		tmp_b = (*b)->next;
		//Check if a double swap is possible, if it is perform swap and then double rotate
		if ((!check_sort_chunks(a, 1) && !check_sort_chunks(b, -1)) && (rotate_count_a < stack_size_a && rotate_count_b < stack_size_b) && ((*a)->index > tmp_a->index) && ((*b)->index < tmp_b->index))
		{
			swap_both(a, b, actions);
			rotate_both(a, b, actions);
			rotate_count_a++;
			rotate_count_b++;
		}
		//Check if a double rotate is possible (opposite condition of double swap)
		else if ((!check_sort_chunks(a, 1) && !check_sort_chunks(b, -1)) && (rotate_count_a < stack_size_a && rotate_count_b < stack_size_b) && ((*a)->index < tmp_a->index) && ((*b)->index > tmp_b->index))
		{
			rotate_both(a, b, actions);
			rotate_count_a++;
			rotate_count_b++;
		}
		//Check for single actions
		else
		{
			//Check swap a and then rotate regardless (always need to rotate, swap or not)
			if (!check_sort_chunks(a, 1) && (rotate_count_a < stack_size_a) && ((*a)->index > tmp_a->index))
				*a = swap_a(*a, actions);
			if (!check_sort_chunks(a, 1) && rotate_count_a < stack_size_a)
			{
				*a = rotate_a(*a, actions);
				rotate_count_a++;
			}
			//Check swap b and then rotate regardless (always need to rotate, swap or not)
			if (!check_sort_chunks(b, -1) && (rotate_count_b < stack_size_b) && ((*b)->index < tmp_b->index))
				*b = swap_b(*b, actions);
			if (!check_sort_chunks(b, -1) && rotate_count_b < stack_size_b)
			{
				*b = rotate_b(*b, actions);
				rotate_count_b++;
			}
		}
	}
	
	
	//Rotate stacks back to starting positions
	//Running find_rotate_direction way more than we need to. Same as check_sort_chunk. Should have vars to hold the return so it only needs to run once per loop or once per function call.!!!!!!!!!!!!!!!!

	//We dont know that the initial top element in still the top element. So we need to iterate though the stack and find the first occurence of an index of the target stack while travelling in the correct direction. This becomes target index.
	int	target_index_a;
	int target_index_b;
	int direction_a;
	int direction_b;
	
	//Find target index (first element belonging to chunk found in stack.)
	target_index_a = find_start_chunk(a, vars, 4);
	target_index_b = find_start_chunk(b, vars, 3);

	while ((*a)->index != target_index_a || (*b)->index != target_index_b)
	{
		direction_a = asc_find_rotate_direction(*a, target_index_a);
		direction_b = asc_find_rotate_direction(*b, target_index_b);
		//Check for double rotate
		if ((direction_a == 1) && (direction_b == 1))
			rotate_both(a, b, actions);
		//Check for double reverse rotate
		else if ((direction_a == -1) && (direction_b == -1))
			rev_rotate_both(a, b, actions);
		//Else single rotate
		else
		{
			//Check direction stack a
			if (direction_a == 1)
				*a = rotate_a(*a, actions);
			else if (direction_a == -1)
				*a = rev_rotate_a(*a, actions);
			//Check direction stack b
			if (direction_b == 1)
				*b = rotate_b(*b, actions);
			else if (direction_b == -1)
				*b = rev_rotate_b(*b, actions);
		}
	}

}

//Function to apply final sorting to chunks
void	final_sort_chunks(t_stack **a, t_stack **b, int **actions, t_vars2 *vars)
{
	int stack_size_a;
	int	target_index;

	stack_size_a = stacksize(*a);
	target_index = vars->chunk_start[4];
	//Push from a to b in descending order
	//First element to push should be start index of chunk 4 up to the end index of chunk 7
	while (*a)
	{
		//If current index is lowest in stack push it
		if ((*a)->index == target_index)
		{
			push_b(b, a, actions);
			target_index++;
		}
		//Else rotate to the lowest index and push it
		//Rotate if faster (return of 1 is rotate, return of -1 is reverse rotate)
		else if (asc_find_rotate_direction(*a, target_index) == 1)
			rotate_a(*a, actions);
		//Reverse rotate if faster
		else
			rev_rotate_a(*a, actions);
	}
	//Push stacks 4,5,6, and 7 back to stack a
	while (stack_size_a > 0)
	{
		push_a(a, b, actions);
		stack_size_a--;
	}

	//Push from b to a in ascending order
	target_index = vars->chunk_end[3];
	while (*b)
	{
		//If current index is lowest in stack push it
		if ((*b)->index == target_index)
		{
			push_a(a, b, actions);
			target_index--;
		}
		//Else rotate to the lowest index and push it
		//Rotate if faster (return of 1 is rotate, return of -1 is reverse rotate)
		else if (asc_find_rotate_direction(*b, target_index) == 1)
			rotate_b(*b, actions);
		//Reverse rotate if faster
		else
			rev_rotate_b(*b, actions);
	}
}

//Function to find the first occurence of an element belonging to the target stack (search based on rotate always?)
//May have to clone and use rotation? Or maybe not. If the first occurence is the first element of stack we dont move. Otherwise we can always find the first index by iterating
//If the chunk overlaps the end and start of the stack this can return a false result. Need to make sure that the last element is not part of chunk. If it is we need to look in reverse from the end.
int	find_start_chunk(t_stack **stack, t_vars2 *vars, int chunk)
{
	t_stack *tmp;
	int	target_index;

	//Set pointer to last element of stack and check if it is part of target chunk. If it is, iterate in reverse to find first element.
	tmp = stacklast(*stack);
	if (tmp->index <= vars->chunk_end[chunk] && tmp->index >= vars->chunk_start[chunk])
	{
		while (tmp->index <= vars->chunk_end[chunk] && tmp->index >= vars->chunk_start[chunk])
		{
			target_index = tmp->index;
			tmp = tmp->previous;
		}
	}
	//Otherwise interate forward to find the first element.
	else
	{
		tmp = *stack;
		while (tmp->index > vars->chunk_end[chunk] || tmp->index < vars->chunk_start[chunk])
			tmp = tmp->next;
		target_index = tmp->index;
	}
	return (tmp->index);
}

//Function to check if a stack is sorted in ascending or descending (1 is ascending, -1 is descending)
//Return of 1 for in order, or 0 for out of order
int	check_sort_chunks(t_stack **stack, int direction)
{
	t_stack *tmp;
	int index;

	tmp = *stack;
	//Set index comparator based on direction
	if (direction == 1)
		index = (*stack)->index - 1;
	else
		index = (*stack)->index + 1;
	while (tmp)
	{
		//For ascending order
		if (direction == 1)
		{
			if (tmp->index != index + 1)
				return (0);
		}
		//For descending order
		else
		{
			if (tmp->index != index - 1)
				return (0);
		}
		index = tmp->index;
		tmp = tmp->next;
	}
	return (1);
}








/*
Approach for chunk sort:

To Do:

	- Write algorithm to create chunks
	- Write sorting tools
	- Apply optimizations
	- Test with different settings

Create chunks:

1. Push the smallest 50% of values to stack b. (Max actions for size=500: 500)
2. Push the smallest 50% of values to stack b. (Max actions for size=500: 250)
3. Push the smallest 50% of values to stack b. (Max actions for size=500: 125)
4. Push the top chunk of stack b back to stack a. (Max actions for size=500: 77.5)
5. Push the largest 50% of values of top chunk of stack b to stack a. (Max actions for size=500: 125)
6. Push the top chunk of stack b to stack a. (Max actions for size=500: 77.5)
	- Now there are 4 chunks containing 50% of total values in the correct chunk order in stack a.
7. Push the largest 50% of values from the remaining chunk in stack b to stack a. (Max actions for size=500: 250)
8. Push the largest 50% of values from the remaining chunk in stack b to stack a. (Max actions for size=500: 125)
9. Push the top chunk from stack a to stack b. (Max actions for size=500: 77.5)
10. Push the smallest 50% of values from the top chunk in stack a to stack b. (Max actions for size=500: 125)
11. Push the top chunk from stack a to stack b. (Max actions for size=500: 77.5)
	- Now there are 8 equal chunks, with 4 in each stack. All chunks are in the correct relative position.

Maximum total actions to complete all of these steps: 1810 = (500 + 250 + 125 + 77.5 + 125 + 77.5 + 250 + 125 +77.5 + 125 + 77.5)

Apply sorting:

12. Run through both stacks between 1 to n times, running swap operations if values are not in order. (Max actions for size=500: between 500-1000)
	- Be sure only to swap within chunks. Will need to save the starting/ending index of each chunk to ensure no mistakes are made.
	- This step may be waste of time, or may be a good way to save time. Will have to test this. Also make sure I can adjust how many times I run through.
	- Make use of double rotate, and double swap when possible during this step.
13. Push values from each chunk in stack a to stack b in order.
14. Push sorted chunks back to stack a in order.
15. Push values from each chunk in stack b to stack a in order.

For steps 13-15, it may be beneficial to do double operations, and end up with two "interlaced stacks" that can then be recombined in order.

The stack should now be sorted, in the correct position, with all values in stack a.

Find optimization for the sorting steps!!!!!!!
Odd sized stacks, and stack sizes not easily divisible by 2 will cause problems. Need to address this.


Example flow:
Smaller number indicates largest values

Starting state:
***********************

	A: | 1 - 100% |
	B: | empty |

***********************

1.	A: | 1 - 50% |
	B: | 2 - 50% |

2.	A: | 1 - 25% |
	B: | 2 - 25% | 3 - 50% |

3.	A: | 1 - 12.5% | 
	B: | 2 - 12.5% | 3 - 25% | 4 - 50% |

4.	A: | 2 - 12.5% | 1 - 12.5% |
	B: | 3 - 25% | 4 - 50% |

5.	A: | 3 - 12.5% | 2 - 12.5% | 1 - 12.5% |
	B: | 4 - 12.5% | 5 - 50% |

6.	A: | 4 - 12.5% | 3 - 12.5% | 2 - 12.5% | 1 - 12.5% |
	B: | 5 - 50% |

7.	A: | 5 - 25% | 4 - 12.5% | 3 - 12.5% | 2 - 12.5% | 1 - 12.5% |
	B: | 6 - 25% |

8.	A: | 6 - 12.5% | 5 - 25% | 4 - 12.5% | 3 - 12.5% | 2 - 12.5% | 1 - 12.5% |
	B: | 7 - 12.5% |

9.	A: | 5 - 25% | 4 - 12.5% | 3 - 12.5% | 2 - 12.5% | 1 - 12.5% |
	B: | 6 - 12.5% | 7 - 12.5% |

10. A: | 5 - 12.5% | 4 - 12.5% | 3 - 12.5% | 2 - 12.5% | 1 - 12.5% |
	B: | 6 - 12.5% | 7 - 12.5% | 8 - 12.5% |

11. A: | 4 - 12.5% | 3 - 12.5% | 2 - 12.5% | 1 - 12.5% |
	B: | 5 - 12.5% | 6 - 12.5% | 7 - 12.5% | 8 - 12.5% |

*/
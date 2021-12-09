/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 13:54:33 by jgoad             #+#    #+#             */
/*   Updated: 2021/12/07 18:20:28 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

//Algorithm to sort using chunks


void	chunk_sort(t_stack **a, t_stack **b, int **actions)
{
	t_vars2 *vars;

	//Initialize vars struct
	vars = NULL;
	vars = chunk_init_vars(a, vars);

	//Split raw stack input into quasi-sorted chunks
	create_chunks(a, b, actions, vars);

	//Apply sorting
}

//Function to initialize vars struct
//THIS WILL BREAK FOR ANY STACKS UNDER SIZE 8!!!!!!!!
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
//Might want to dynamically allocate/fill this array as we create chunks to prevent errors??

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
	while (i >= 0)
	{
		push_b(b, a, actions);
		i--;
	}
	//10. Push the smallest 50% of values from the top chunk in stack a to stack b. (chunk 2)
	i = vars->chunk_size[2] + vars->chunk_size[3]; //Check notation on this line. Need to iterate through both chunks to seperate
	rotate_count = 0;
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
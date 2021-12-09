/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_control.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 16:34:02 by jgoad             #+#    #+#             */
/*   Updated: 2021/11/28 12:38:06 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

//This file contains function that control the sorting logic of the program


/* Initial basic sort approach:
	- Find the starting element that provides the largest set of ascending values.
	- Keep these values in stack a, and push all other values to stack b.
	- Find the appropriate spot in stack a to move values back from stack b.
	- Repeat until all values are in stack a.

	Add some optimization logic to this process to choose steps with less actions.

	Finally, apply recursion to find the quickest path.

	Repeat process with different algorithms, and apply recurion to those.

	Compare results, and return the solution with the least actions taken.

	Possibly hard code or create a seperate algorithm for small sets (5 or less values)
*/




//Function to control sorting logic
void	sort_control(t_stack **a, t_stack **b, t_stack **sorted)
{
	//Create an array of integers to hold actions (each action is indexed to an integer value)
	int actions[10000];
	int *p_act = actions;
	//Initialize array to 0;
	int i;

	i = -1;
	while (i++ < 10000)
		actions[i] = 0;

	//Run ascending value algorithm
	ascending_index(a, b, sorted, &p_act);

	//Print out actions list;
	i = 0;
	printf("\nActions list:\n");
	while (actions[i] != 0)
	{
		printf("%d ", actions[i]);
		i++;
	}
	printf("\nAction count: %d\n", i);
	//Print our current stacks to see results
	t_stack *tmp;
	tmp = *a;
	printf("\nStack a after pushing:\n");
	while (tmp)
	{
		printf("Index is: %d\n", tmp->index);
		printf("Value is: %d\n\n", tmp->value);
		tmp = tmp->next;
	}

	tmp = *b;
	printf("\nStack b after pushing:\n");
	while (tmp)
	{
		printf("Index is: %d\n", tmp->index);
		printf("Value is: %d\n\n", tmp->value);
		tmp = tmp->next;
	}
	
}

//Control function for ascending index sorting algorithm
void	ascending_index(t_stack **a, t_stack **b, t_stack **sorted, int **actions)
{
	int start_index;
	int stack_size;
	int i;
	int current_index;
	int index_limit;
	t_stack *next_node;

	sorted = NULL;
	stack_size = stacksize(*a);
	i = 0;

	//test actions
	(*actions)[0] = 12345;
	//Find the starting value that allows the most values to be kept in stack a
	start_index = find_start(*a);

	//Rotate a until the start index is found
	while ((*a)->index != start_index)
		*a = rotate_a(*a, actions);

	//Push values to stack b
	current_index = start_index;
	while(i < stack_size)
	{
		//If index of current node is greater than current highest index, update current_index and rotate stack a
		if ((*a)->index > current_index)
		{
			current_index = (*a)->index;
			*a = rotate_a(*a, actions);
		}
		//If index of current node is less than current higheest index, push the node to stack b. No need to rotate stack a in this case, as we have moved to the next node by deleting the top noded
		else if ((*a)->index < current_index)
			push_b(b, a, actions);
		//If no conditions are met (only the first iteration) rotate a 
		else
			*a = rotate_a(*a, actions);
		i++;
	}
	
	//Push values back to stack a
	while (*b)
	{
		//Check index value of top node of stack b, rotate a until the correct position is found
		//Check the index of the next and previous nodes of a, if b fits in between them, push b

		//If b->index is greater than a->index, rotate a
		while (*b && (*b)->index > (*a)->index)
		{
			//Find current highest index in stack a
			index_limit = find_highest(*a); 
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
			else if ((*a)->index == index_limit)
			{
				*a = rotate_a(*a, actions);
				push_a(a, b, actions);
			}
			//If b does not fit rotate
			else
				*a = rotate_a(*a, actions);
		}

		//If b->index is less than a->index reverse rotate a
		while (*b && (*b)->index < (*a)->index)
		{
			//Find current lowest index in stack a
			index_limit = find_lowest(*a); 

			//Set next_done to the value of the previous node in stack a (which is the last node in the current stack)
			next_node = stacklast(*a);
		
			//Check if b fits between current node and the previous node
			if ((*b)->index > next_node->index)
				push_a(a,b, actions);
			//If index of b is lower than all of the indices in stack a, we need to place it above the lowest current index in stack a
			else if ((*a)->index == index_limit)
				push_a(a, b, actions);
			else
				*a = rev_rotate_a(*a, actions);
		}
	}
	
	//Rotate a back to index 0 on top
	//Can optimize this to see which direction is the fastest
	while ((*a)->index != 0)
		*a = rotate_a(*a, actions);
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

//Find the integer with the most integers that follow that are consecutively larger.
//Way too many variables. Use a struct to hold vars.
//Do not want to include actions for this function
int	find_start(t_stack *a)
{
	int	count;
	int count_tmp;
	int	start_index;
	int current_index;
	int inner;
	int outer;
	int stack_size;

	//Set initial count to 1
	count = 1;
	//Get stacksize to use as loop control
	stack_size = stacksize(a);
	//Inialize loop control variables
	outer = 0;
	inner = 0;

	//Run rotate until we have completed one complete loop of the stack
	while (outer < stack_size)
	{
		//Reset count_tmp
		count_tmp = 1; //Count always starts at one, because minimum set size is one element
		//Reset current index
		current_index = a->index;
		inner = 0;
		//Run rotate until we have done one complete loop of the stack
		while (inner < stack_size)
		{
			//If index of current element is higher than previous index, increment count and set new highest index
			if (a->index > current_index)
			{
				count_tmp++;
				current_index = a->index;
			}
			//Rotate once to next node
			a = rotate_a(a, NULL);
			inner++;
		}
		//If count from inner loop is higher than previous count, set new highest count and new start index
		if (count_tmp > count)
		{
			count = count_tmp;
			start_index = a->index; //Since we end on the element we started on there is no need to store our starting index for inner loop
		}
		//Rotate once to test next start position
		a = rotate_a(a, NULL);
		outer++;
	}
	//Return the best starting index
	return (start_index);
}




/*
	//Testing after finding index
	t_stack *tmp;
	tmp = *a;
	printf("\nAfter rotate:\n");
	while (tmp)
	{
		printf("Index is: %d\n", tmp->index);
		printf("Value is: %d\n\n", tmp->value);
		tmp = tmp->next;
	}
	*/
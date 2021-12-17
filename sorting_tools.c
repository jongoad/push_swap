/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sorting_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:39:05 by jgoad             #+#    #+#             */
/*   Updated: 2021/12/14 19:09:51 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

//This file contains the allowed sorting tools.

//Should add error handling for attempting invalid actions (e.g. empty stack)

//Swap a
t_stack	*swap_a(t_stack *a, int **actions)
{
	int tmp1_index;
	int tmp1_value;
	int tmp2_index;
	int tmp2_value;

	//Only do something if there are at least two populated nodes
	if (a && a->next)
	{
		//Save values from first node
		tmp1_index = a->index;
		tmp1_value = a->value;

		//Move to second node
		a = a->next;
		tmp2_index = a->index;
		tmp2_value = a->value;
		a->index = tmp1_index;
		a->value = tmp1_value;

		//Move back to first node
		a = a->previous;
		a->index = tmp2_index;
		a->value = tmp2_value;

		//Set action
		if (actions)
		{
			*(*actions) = 1;
			(*actions)++;
		}
	}
	return (a);
}

//Swap b
t_stack	*swap_b(t_stack *b, int **actions)
{
	int tmp1_index;
	int tmp1_value;
	int tmp2_index;
	int tmp2_value;

	//Only do something if there are at least two populated nodes
	if (b && b->next)
	{
		//Save values from first node
		tmp1_index = b->index;
		tmp1_value = b->value;

		//Move to second node
		b = b->next;
		tmp2_index = b->index;
		tmp2_value = b->value;
		b->index = tmp1_index;
		b->value = tmp1_value;

		//Move back to first node
		b = b->previous;
		b->index = tmp2_index;
		b->value = tmp2_value;

		//Set action
		if (actions)
		{
			*(*actions) = 2;
			(*actions)++;
		}
	}
	return (b);
}

//Swap both
void	swap_both(t_stack **a, t_stack **b, int **actions)
{
	*a = swap_a(*a, NULL);
	*b = swap_b(*b, NULL);

	
	//Set action
	if (actions)
	{
		*(*actions) = 3;
		(*actions)++;
	}

}

//Push a (push from b to a)
void	push_a(t_stack **a, t_stack **b, int **actions)
{
	t_stack *ptr;
	
	ptr = *b;
	//Only run there is at least one node in b
	if (*b)
	{
		//Create new node at beginning of stack a using values from first node of stack b
		stackadd_front(a, stacknew(ptr->value, ptr->index, NULL));
		//Delete node from top of stack b
		stackdeltop(b);

		//Set action
		if (actions)
		{
			*(*actions) = 4;
			(*actions)++;
		}
	}
}

//Push b (push from a to b)
void	push_b(t_stack **b, t_stack **a, int **actions)
{
	t_stack *ptr;

	ptr = *a;
	//Only run there is at least one node in a
	if (*a)
	{
		//Create new node at beginning of stack b using values from first node of stack a
		stackadd_front(b, stacknew(ptr->value, ptr->index, NULL));
		//Delete node from top of stack a
		stackdeltop(a);

		//Set action
		if (actions)
		{
			*(*actions) = 5;
			(*actions)++;
		}
	}
}

//Rotate a
t_stack	*rotate_a(t_stack *a, int **actions)
{
	int	tmp1_index;
	int	tmp1_value;
	int	tmp2_index;
	int	tmp2_value;
	t_stack *ptr;

	ptr = a;
	if (a)
	{
		//Store values of first node
		tmp1_index = ptr->index;
		tmp1_value = ptr->value;

		//Iterate to end of stack
		ptr = stacklast(a);
		//Copy values of last node and then replace with values of first node
		while (ptr)
		{
			//Store values of current node
			tmp2_index = ptr->index;
			tmp2_value = ptr->value;
			//Apply values of previous node
			ptr->index = tmp1_index;
			ptr->value = tmp1_value;
			//Reset buffers to current node values
			tmp1_index = tmp2_index;
			tmp1_value = tmp2_value;

			//Iterate to previous node
			ptr = ptr->previous;
		}
		//Set action
		if (actions)
		{
			*(*actions) = 6;
			(*actions)++;
		}
	}
	//Return new stack
	return (a);
}

//Rotate b
t_stack	*rotate_b(t_stack *b, int **actions)
{
	int	tmp1_index;
	int	tmp1_value;
	int	tmp2_index;
	int	tmp2_value;
	t_stack *ptr;

	ptr = b;
	if (b)
	{
		//Store values of first node
		tmp1_index = ptr->index;
		tmp1_value = ptr->value;

		//Iterate to end of stack
		ptr = stacklast(b);
		//Copy values of last node and then replace with values of first node
		while (ptr)
		{
			//Store values of current node
			tmp2_index = ptr->index;
			tmp2_value = ptr->value;
			//Apply values of previous node
			ptr->index = tmp1_index;
			ptr->value = tmp1_value;
			//Reset buffers to current node values
			tmp1_index = tmp2_index;
			tmp1_value = tmp2_value;

			//Iterate to previous node
			ptr = ptr->previous;
		}
		//Set action
		if (actions)
		{
			*(*actions) = 7;
			(*actions)++;
		}
	}
	//Return new stack
	return (b);
}

//Rotate both
void	rotate_both(t_stack **a, t_stack **b, int **actions)
{
	*a = rotate_a(*a, NULL);
	*b = rotate_b(*b, NULL);

	
	//Set action
	if (actions)
	{
		*(*actions) = 8;
		(*actions)++;
	}
}

//Reverse rotate a
t_stack	*rev_rotate_a(t_stack *a, int **actions)
{
	int	tmp1_index;
	int	tmp1_value;
	int	tmp2_index;
	int	tmp2_value;
	t_stack *ptr;

	ptr = a;
	//Only run if there are at least to nodes
	if (a && a->next)
	{
		//Iterate to last node and copy values
		ptr = stacklast(a);
		tmp1_index = ptr->index;
		tmp1_value = ptr->value;

		//Reset ptr to first node
		ptr = a;
		//Iterate through stack, moving values of each node down by one
		while (ptr)
		{
			//Store values of current node
			tmp2_index = ptr->index;
			tmp2_value = ptr->value;
			//Apply values of previous node
			ptr->index = tmp1_index;
			ptr->value = tmp1_value;
			//Reset buffers to current node values
			tmp1_index = tmp2_index;
			tmp1_value = tmp2_value;

			//Iterate to next node
			ptr = ptr->next;
		}
		//Set action
		if (actions)
		{
			*(*actions) = 9;
			(*actions)++;
		}
	}
	//Return new stack
	return (a);
}

//Reverse rotate b
t_stack	*rev_rotate_b(t_stack *b, int **actions)
{
	int	tmp1_index;
	int	tmp1_value;
	int	tmp2_index;
	int	tmp2_value;
	t_stack *ptr;

	ptr = b;
	//Only run if there are at least to nodes
	if (b && b->next)
	{
		//Iterate to last node and copy values
		ptr = stacklast(b);
		tmp1_index = ptr->index;
		tmp1_value = ptr->value;

		//Reset ptr to first node
		ptr = b;
		//Iterate through stack, moving values of each node down by one
		while (ptr)
		{
			//Store values of current node
			tmp2_index = ptr->index;
			tmp2_value = ptr->value;
			//Apply values of previous node
			ptr->index = tmp1_index;
			ptr->value = tmp1_value;
			//Reset buffers to current node values
			tmp1_index = tmp2_index;
			tmp1_value = tmp2_value;

			//Iterate to next node
			ptr = ptr->next;
		}
		//Set action
		if (actions)
		{
			*(*actions) = 10;
			(*actions)++;
		}
	}
	//Return new stack
	return (b);
}

//Reverse rotate both
void	rev_rotate_both(t_stack **a, t_stack **b, int **actions)
{
	*a = rev_rotate_a(*a, NULL);
	*b = rev_rotate_b(*b, NULL);

	//Set action
	if (actions)
	{
		*(*actions) = 11;
		(*actions)++;
	}
}

/* Action Index Numbers - To be used in int array to simply store actions

0 - no action taken
1 - sa - swap a
2 - sb - swap b
3 - ss - swap both
4 - pa - push a
5 - pb - push b
6 - ra - rotate a
7 - rb - rotate b
8 - rr - rotate both
9 - rra - reverse rotate a
10 - rrb - reverse rotate b
11 - rrr - reverse rotate both

Double actions require passing actions to the intial function, but we do not want to record individual actions
*/


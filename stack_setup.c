/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_setup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 16:35:09 by jgoad             #+#    #+#             */
/*   Updated: 2021/12/02 14:57:38 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

//This file contains functions to setup the stacks for the sorting algorithms

//Function to control stuff and things
void	setup_control(t_stack **a, t_stack **b)
{
	t_stack *sorted;

	//Initialize sorted to null to prevent bad access
	sorted = NULL;
	//Create a copy of stack a to be sorted
	sorted = clone_stack(*a, &sorted);
	//Sort stack into solve state
	sorted = stack_sort(sorted);
	//Assign index values to solved stack
	sorted = find_index(sorted);
	//Transfer index values to stack a
	*a = apply_index(*a, sorted);
	//Send all current information to sorting control
	sort_control(a, b);
}

//Function to clone a stack (CAN CHANGE THIS TO RETURN THE LIST WITHOUT HAVING TO FEED THE LIST IN)
//Need to apply the proper previous pointers in the DST stack, how to get the address ?
t_stack	*clone_stack(t_stack *src, t_stack **dst)
{
	t_stack *tmp;
	t_stack *tmp_previous;

	tmp = src;
	while (tmp)
	{
		//For first element pass NULL for previous pointer value
		if (!*dst)
			stackadd_back(dst, stacknew(tmp->value, 0, NULL));
		else
		{	tmp_previous = stacklast(*dst);
			stackadd_back(dst, stacknew(tmp->value, 0, tmp_previous));
		}
		tmp = tmp->next;
	}
	return (*dst);
}

//Function to apply index numbers to sorted stack
t_stack *find_index(t_stack *sorted)
{
	int i;
	t_stack *tmp;

	i = 0;
	tmp = sorted;
	while (tmp)
	{
		tmp->index = i;
		tmp = tmp->next;
		i++;
	}
	return (sorted);
}

//Function to apply indices to stack a
t_stack *apply_index(t_stack *dst, t_stack *src)
{
	t_stack *tmp_dst;
	t_stack *tmp_src;

	tmp_dst = dst;
	while (tmp_dst)
	{
		tmp_src = src;
		while (tmp_src)
		{
			if (tmp_dst->value == tmp_src->value)
				tmp_dst->index = tmp_src->index;
			tmp_src = tmp_src->next;
		}
		tmp_dst = tmp_dst->next;
	}
	return (dst);
}
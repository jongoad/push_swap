/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_control.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 16:34:02 by jgoad             #+#    #+#             */
/*   Updated: 2021/12/09 15:09:25 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

//This file contains function that control the sorting logic of the program

//Function to control sorting logic
void	sort_control(t_stack **a, t_stack **b)
{
	//Create an array of integers to hold actions (each action is indexed to an integer value)
	int actions[20000];
	int *p_act = actions;
	//Initialize array to 0;
	int i;

	//Array of strings to hold instructions
	//char	*action_translate[11] = {"sa", "sb", "ss", "pa", "pb", "ra", "rb", "rr", "rra", "rrb", "rrr"};

	i = -1;
	while (i++ < 20000)
		actions[i] = 0;

	//Run ascending value algorithm
	//ascending_index(a, b, &p_act);
	chunk_sort(a, b, &p_act);

	//Print out stack size
	//printf("\nStack size is: %d\n", stacksize(*a));

	//Print out actions list;
	i = 0;
	//printf("\nActions list:\n");
	while (actions[i] != 0)
	{
		//printf("%s", action_translate[actions[i] - 1]);
		//Print a newline unless the next action is empty
		//if (actions[i + 1] != 0)
		//	printf("\n");
		i++;
	}
	
	printf("\nAction count: %d\n", i);
	//Print our current stacks to see results
	t_stack *tmp;
	int count = 0;
	tmp = *a;
	printf("\nStack a after pushing:\n");
	while (tmp)
	{
		printf("Index is: %d\n", tmp->index);
		printf("Value is: %d\n\n", tmp->value);
		tmp = tmp->next;
		count++;
	}
	printf("Number of element in stack A: %d\n\n", count);
	count = 0;
	tmp = *b;
	printf("\nStack b after pushing:\n");
	while (tmp)
	{
		printf("Index is: %d\n", tmp->index);
		printf("Value is: %d\n\n", tmp->value);
		tmp = tmp->next;
		count++;
	}
	printf("Number of element in stack B: %d\n\n", count);
}
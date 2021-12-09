/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:43:59 by jgoad             #+#    #+#             */
/*   Updated: 2021/11/25 19:05:12 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

//This file contains the functions required to parse the input and convert to a linked list

//Function to check an individual string for invalid characters, convert to a value, and confirm it is valid
int	parse_integer(char *str, int *n)
{
	int	i;

	i = 0;
	//Check to ensure that string is numeric
	if (str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	//Convert to long and check if valid integer
	*n = ft_atol(str);
	if (*n > INT_MAX || *n < INT_MIN)
		return (0);
	else
		return (1);
}

//Function to check through the integer list and ensure that there are no duplicate values
int	parse_duplicate(t_stack* a)
{
	t_stack *ptr1;
	t_stack *ptr2;

	ptr1 = a;
	while (ptr1)
	{
		ptr2 = ptr1->next;
		while (ptr2)
		{
			if (ptr1->value == ptr2->value)
				return (0);
			ptr2 = ptr2->next;
		}
		ptr1 = ptr1->next;
	}
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:18:19 by jgoad             #+#    #+#             */
/*   Updated: 2021/12/02 14:54:59 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//This file contains the main body of the push_swap program

#include "ft_push_swap.h"


//Main function
int	main(int argc, char *argv[])
{
	int	i;
	int n;
	t_stack *a;
	t_stack *b;
	t_stack *a_ptr;

	i = 1;
	a = NULL;
	b = NULL;
	//Ensure that at least 1 argument has been passed
	if (argc < 2)
		return (write_err());
	//Iterate through argv array, parse each element and add to list
	while (i < argc)
	{
		//Parse integer string and return error if invalid
		if (!parse_integer(argv[i], &n))
		{
			//Can combine error writing and free list to save lines
			//free_list();
			return (write_err());
		}
		//If integer is valid add to linked list
		else
		{
			if (!a)
			{
				a_ptr = stacknew(n, 0, NULL);
				stackadd_back(&a, a_ptr);
			}
			else
			{
				a_ptr = stacknew(n, 0, a_ptr);
				stackadd_back(&a, a_ptr);
			}
		}
		i++;
	}
	//Check for duplicates
	if (!parse_duplicate(a))
		return (write_err());

	//Pass lists to sorting logic control function
	setup_control(&a, &b);
	return (0);
}

//Create a minimum viable product as starting point and then tweak
//How to create branching behaviour for recursive solving?

//We can capitalize on double actions by splitting chunks into two stacks?
//Push half of values into stack b immediately??

//If all of the values are pushed to stack b, all of the numbers then have to be pushed back.
//Would it be more efficient to do some the sorting in stack and stack b at the same time?

//Can i intentionally "prime" stacks into a position where the most efficient operation is a double action?

//What if I sort stack a to have largest numbers at the top, and stack b to have smallest numbers at the top?
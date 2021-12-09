/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rndnum.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 14:44:18 by jgoad             #+#    #+#             */
/*   Updated: 2021/12/07 17:14:30 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Generate random number stacks

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main (void)
{
	int stack[10];
	int i;

	i = 0;
	srand(time(NULL)); //Initialize seed

	//Fill array with integers
	while (i < 24)
	{
		//Check if number is duplicate
		stack[i] = rand() % 50;
		i++;
	}
	i = 0;
	while (i < 24)
	{
		printf("%d ", stack[i]);
		i++;
	}
	printf ("\n");
	return (0);
}

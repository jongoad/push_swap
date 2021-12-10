/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 13:59:41 by jgoad             #+#    #+#             */
/*   Updated: 2021/12/10 15:32:37 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

//Function to write error and handle memory clearing
int	write_err(void)
{
	//ft_putstr_fd("Error\n", 1);
	return (1);
}

//Convert a string to a long
long	ft_atol(const char *str)
{
	long long	num;
	long long	sign;

	num = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		num = (num * 10) + (*str - '0');
		str++;
		if ((num * sign) > INT_MAX)
			return (LONG_MAX);
		if ((num * sign) < INT_MIN)
			return (LONG_MIN);
	}
	return (num * sign);
}

//Function to sort values into the final solve state.
t_stack	*stack_sort(t_stack *solved)
{
	int 	tmp_value;
	int		tmp_index;
	t_stack	*tmp1;
	t_stack	*tmp2;

	tmp1 = solved;
	while (tmp1)
	{
		tmp2 = tmp1;
		while (tmp2)
		{
			if (tmp1->value > tmp2->value)
			{
				tmp_value = tmp1->value;
				tmp_index = tmp1->index;
				tmp1->value = tmp2->value;
				tmp1->index = tmp2->index;
				tmp2->value = tmp_value;
				tmp2->index = tmp_index;
			}
			tmp2 = tmp2->next;
		}
		tmp1 = tmp1->next;
	}
	return (solved);
}
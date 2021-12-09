/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/15 16:38:05 by jgoad             #+#    #+#             */
/*   Updated: 2021/10/25 16:52:01 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_count_digits(int nbr)
{
	int	count;

	count = 0;
	if (nbr <= 0)
		count = 1;
	while (nbr)
	{
		count++;
		nbr = nbr / 10;
	}
	return (count);
}

char	*ft_itoa(long n)
{
	int		count;
	char	*str;
	long	nbr;

	count = ft_count_digits(n);
	nbr = n;
	str = (char *)malloc(sizeof(char) * (count + 1));
	if (!str)
		return (NULL);
	if (nbr < 0)
	{
		str[0] = '-';
		nbr = -nbr;
	}
	if (nbr == 0)
		str[0] = '0';
	str[count--] = '\0';
	while (nbr)
	{
		str[count] = (nbr % 10) + '0';
		count--;
		nbr = (nbr / 10);
	}
	return (str);
}

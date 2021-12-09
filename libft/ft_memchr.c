/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/15 10:49:52 by jgoad             #+#    #+#             */
/*   Updated: 2021/09/20 18:05:45 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t		i;
	const char	*p_s;

	i = 0;
	p_s = (const char *)s;
	while (i < n)
	{
		if (*(p_s + i) == (char)c)
			return ((void *)(p_s + i));
		i++;
	}
	return (NULL);
}

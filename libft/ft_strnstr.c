/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/15 11:11:57 by jgoad             #+#    #+#             */
/*   Updated: 2021/09/20 18:16:23 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	unsigned int	i;
	unsigned int	count;

	count = 0;
	if (*needle == '\0')
		return ((char *)haystack);
	while (*haystack && count < len)
	{
		count = 0;
		i = 0;
		while (*(haystack + i) == needle[i] && count < len)
		{	
			if (needle[i + 1] == '\0')
				return ((char *)haystack);
			i++;
			count++;
		}
		haystack++;
		len --;
	}
	return (NULL);
}

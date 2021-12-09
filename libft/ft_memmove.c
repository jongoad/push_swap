/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 15:07:58 by jgoad             #+#    #+#             */
/*   Updated: 2021/09/20 18:06:57 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*p_dst;
	char	*p_src;

	p_dst = (char *)dst;
	p_src = (char *)src;
	if (dst == src)
		return (dst);
	if (p_src < p_dst)
	{
		while (len--)
			*(p_dst + len) = *(p_src + len);
		return (dst);
	}
	while (len--)
		*p_dst++ = *p_src++;
	return (dst);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 14:46:21 by jgoad             #+#    #+#             */
/*   Updated: 2021/09/20 18:00:57 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*p_lst;
	t_list	*p_next;

	if (!lst || !del)
		return ;
	p_lst = *lst;
	while (p_lst)
	{
		p_next = p_lst->next;
		ft_lstdelone(p_lst, del);
		p_lst = p_next;
	}
	*lst = NULL;
}

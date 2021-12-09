/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llist_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 15:33:31 by jgoad             #+#    #+#             */
/*   Updated: 2021/11/28 18:22:29 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

//Initialize a new stack
t_stack	*stacknew(int value, int index, t_stack *previous)
{
	t_stack	*node;

	node = (t_stack *)malloc(sizeof(t_stack));
	if (!node)
		return (NULL);
	node->value = value;
	node->index = index;
	node->previous = previous;
	node->next = NULL;
	return (node);
}

//Add a node to the back of the stack
void	stackadd_back(t_stack **lst, t_stack *new)
{
	t_stack	*ptr_lst;

	//If list already exists
	if (*lst)
	{
		ptr_lst = stacklast(*lst);
		ptr_lst->next = new;
	}
	//If no list exists yet, set previous to NULL
	else
		*lst = new;
	return ;
}

//Add a node to the front of the stack
void	stackadd_front(t_stack **lst, t_stack *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	//If there are any nodes in the stack, set the previous value for the first node before it is pushed
	if (*lst)
		(*lst)->previous = new;
	*lst = new;
	return ;
}

//Count number of nodes in stack
int	stacksize(t_stack *lst)
{
	int		count;
	t_stack	*ptr;

	count = 0;
	ptr = lst;
	while (ptr)
	{
		count++;
		ptr = ptr->next;
	}
	return (count);
}

//Get pointer to last node of stack
t_stack	*stacklast(t_stack *lst)
{
	if (!lst)
		return (NULL);
	while (lst)
	{
		if (lst->next == NULL)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

//Clear the stack
void	stackclear(t_stack **lst)
{
	t_stack	*p_lst;
	t_stack	*p_next;

	if (!lst)
		return ;
	p_lst = *lst;
	while (p_lst)
	{
		p_next = p_lst->next;
		stackdelone(p_lst);
		p_lst = p_next;
	}
	*lst = NULL;
}

//Delete one node from stack
void	stackdelone(t_stack *lst)
{
	if (!lst)
		return ;
	free(lst);
}

//Delete one node from stack top of stack
void	stackdeltop(t_stack **lst)
{
	t_stack *ptr;
	t_stack *ptr_next;

	if (!*lst)
		return ;
	ptr = *lst;
	ptr_next = ptr->next;
	
	//Free current node
	free(*lst);

	//Set next node to be the first node
	*lst = ptr_next;
	//Set previous link for first node to NULL if first node exists
	if (*lst)
		(*lst)->previous = NULL;
}
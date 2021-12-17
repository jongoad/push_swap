/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newsort.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 15:54:45 by jgoad             #+#    #+#             */
/*   Updated: 2021/12/16 15:58:57 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_push_swap.h"

//While stack is not sorted repeat this in a loop:
//Keep track of highest sorted value and do not push that element or anything lower

    //Find midpoint of stack a
    //Push lowest 50% of values to b
    //Push highest 50% of values back to a
    //Repeat until n elements are left in a
    //Push elements back to a in order, then rotate them to the bottom of stack



void    recursive_stack_sort(t_stack **a, t_stack **b, int **actions, int first)
{
    int sort_end; //Need to inialize in a way that it wont be reinitialized every timefunction is called recursively
    static int tot_count;
    int mid;
    int n;
    t_stack *tmp;

    //Initialize sort_end only on first iteration only
    if (first == 1)
    {
        sort_end = 0;
        tot_count = stacksize(*a);
        first = 0;
    }
    else
    {
        tmp = stacklast(*a);
        sort_end = tmp->index + 1;
    }
    //Tuning variable for number of elements to be left in stack b
    //This will not work at all for small stacks
    n = 12;

    //Base case for recursion control
    //A must be sorted AND contain all of the elements
    //On the first iteration the stack will contain all elements to this expression will evaluate to true!!!!!!!!!!!!
    //How else can I control for base case? I could use tot_count and add a parameter to is_sorted to check if it is sorted AND contains all elements
    while (!is_sorted(a, tot_count))
    {
        //Set midpoint I THINK THIS IS THE BREAKING POINT, NEED TO ADJUST THIS CONTROL. STILL HAVING NO ELEMENTS PUSHED TO B IN SOME CASES
        //If there are less than a certain number of elements unsorted, need to push all and sort

        //If there are less than n elements to be sorted, push all of the elements

        //Count of unsorted is equal to tot_size - sort_end
        if ((tot_count - sort_end + 1) <= n)
        {
            while ((*a)->index >= sort_end)
                push_b(b, a, actions);
        }
        else
        {
            mid = (stacksize(*a) / 2) + (sort_end / 2);
            //Push lowest 50% of elements to stack. THIS NEEDS TO DO REV ROTATE AS WELL TO BE MORE EFFICIENT??
            //find lowest does not work because we have sorted lower elements to stack A already
            //Need to check that the lowest NON sorted value is mid
            //Sometimes when the stack is almost sorted, mid is set to a value with no unsorted values below, thus nothing is pushed to b, and bad access occurs
            //Need to ensure that there is always a  value to push
            while (find_lowest_above_sort_end(*a, sort_end) != mid)
            {
                if ((*a)->index < mid && (*a)->index >= sort_end)
                    push_b(b, a, actions);
                else
                    *a = rotate_a(*a, actions);
            }
        }

     

        //Push elements back to stack a until there are n remaining
        while (stacksize(*b) > n)
        {
            mid = (find_highest(*b) + find_lowest(*b)) / 2;
            //Find the lowest element in stack and add (n - 1)
            while (find_highest(*b) > mid)
            {
                if ((*b)->index > mid)
                    push_a(a, b, actions);
                else
                    *a = rotate_a(*a, actions);
            }
        }


        //Rotate so sort_end is the last element in stack a if not first iteration

        if (sort_end != 0)
        {
            tmp = stacklast(*a);
            while (tmp->index != sort_end - 1)
            {
                *a = rev_rotate_a(*a, actions);
                tmp = stacklast(*a);
            }
        }
        //Set sort end to the highest index in stack b
        sort_end = find_highest(*b) + 1;
        int i = sort_end - 1;

        //Push back to stack a in order
        while (*b)
        {   
            //Find the highest element in stack b and push, repeat till b is empty
            if ((*b)->index == i)
            {
                push_a(a, b, actions);
                i--;
            }
            else if (chunk_find_rotate_direction(*b, i) == -1)
                *b = rev_rotate_b(*b, actions);
            else if (chunk_find_rotate_direction(*b, i) == 1)
                *b = rotate_b(*b, actions);
        }

        //Rotate stack *a so all of the sorted values are on bottom of the stack
        //Rotate until the element below (sort_end - 1) is on top of stack
        tmp = stacklast(*a);
        while ((*a)->index != sort_end - 1)
        {
            *a = rotate_a(*a, actions);
            tmp = stacklast(*a);
        }
        //rotate once more to push sort_end to bottom of stack
        *a = rotate_a(*a, actions);


        //Call function recursively
        recursive_stack_sort(a, b, actions, first);
    }
    //If sort is finished place index 0 on top of stack
    if (is_sorted(a, tot_count))
    {
        //Rotate back to start index
        while ((*a)->index != 0)
        {
            if (chunk_find_rotate_direction(*a, 0) == -1)
                *a = rev_rotate_a(*a, actions);
            else if (chunk_find_rotate_direction(*a, 0) == 1)
                *a = rotate_a(*a, actions);
        }
    }
}

//Function to check if stack is sorted
int is_sorted(t_stack **a, int tot_count)
{
    int top_a;
    t_stack *tmp;
    int i;
    int stack_size;
    int is_sort;

    top_a = (*a)->index;
    i = 0;
    stack_size = stacksize(*a);
    is_sort = 1;
    //Rotate to index 0
    while ((*a)->index != 0)
        *a = rotate_a(*a, NULL);

    tmp = *a;
    //Iterate to end of stack ensuring that each element's index is equal to previous element + 1 (by comparing to i)
    while (tmp)
    {
        if (tmp->index != i)
        {
            is_sort = 0;
            break;
        }
        i++;
        tmp = tmp->next;
    }

    //rotate back to top_index
    while ((*a)->index != top_a)
        *a = rotate_a(*a, NULL);
    
    //If stack is sorted AND contains all the elements return 1
    if (stacksize(*a) == tot_count && is_sort == 1)
        return (1);
    else
        return (0);
}


//Function to find the lowest index currently in a stack
int	find_lowest_above_sort_end(t_stack *stack, int sort_end)
{
	int index;

	index = stack->index;
	while (stack)
	{
		if (stack->index < index && stack->index >= sort_end)
			index = stack->index;
		stack = stack->next;
	}
	return (index);
}

/*
Does not seem to be working at all currently
Breaks as soon as more than 25 integers are passsed
*/
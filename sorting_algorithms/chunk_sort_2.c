/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_sort_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 13:34:47 by jgoad             #+#    #+#             */
/*   Updated: 2021/12/14 15:01:13 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_push_swap.h"



//Controller function
void    chunk_sort_new(t_stack **a, t_stack **b, int **actions)
{
    t_vars3 *vars;
    int chunk_count;

    chunk_count = 10;//Odd numbered chunk counts are currently broken.
    //Initialize vars and set the number of chunks to be used
    vars = NULL;
    vars = init_vars_3(a, vars); //May not need this function, can malloc right here
    init_chunk_info(vars, chunk_count);

    //Find the list of elements to be kept in stack a
    chunk_keep_indices(a, vars);

    //Push the remaining elements to stack b in chunks 
    chunk_push_remain(a, b, actions, vars);

    //Push elements from b back to a in order
    chunk_push_order(a, b, actions, vars);

    //Make sure we are rotating back to the start index (optimize with direction?)
    while ((*a)->index != 0)
        *a = rev_rotate_a(*a, actions);
}

//Function to push elements back into stack a in the proper order
void    chunk_push_order(t_stack **a, t_stack **b, int **actions, t_vars3 *vars)
{
    t_stack *tmp;
    
    //Rotate stack a so the largest value is on top (optimize this by checking best direction to rotate). Split this off into seperate function
    int i = 0;
    while (vars->keep_indices[i] != -1)
        i++;
    while ((*a)->index != vars->keep_indices[i - 1])
        *a = rotate_a(*a, actions);
    
    //While there are any elements left in b
    //Need to control for rolling over chunk 1 to n or n to 1
    while (*b)
    {
        //If top element of stack b is greater than top element of stack a (need to control for when there are no larger elements left in list)
        while ((*b) && (*b)->index > (*a)->index)
        {
            //Rotate stack a
            *a = rotate_a(*a, actions);
            //If next element is larger then push, or if top element of b is larger than the largest index in stack a
            if (((*b)->index < (*a)->index) || ((*b)->index > get_largest_index(*a)))
                push_a(a, b, actions);
        }

        //If top element of stack b is less than top element of stack
        while ((*b) && (*b)->index < (*a)->index)
        {
            //Check if the last element on this list is smaller than top of b, if it is then push,  also push if b is smaller than the smallest index in stack a
            tmp = stacklast(*a);
            if (((*b)->index > tmp->index) || ((*b)->index < get_smallest_index(*a) && (*a)->index == get_smallest_index(*a)))
                push_a(a, b, actions);
            //Otherwise reverse rotate
            else 
                rev_rotate_a(*a, actions);
        }
    }
}

//Function to get the largest index in the current stack
int get_largest_index(t_stack *stack)
{
    int stack_size;
    int largest;
    int tmp;

    tmp = stack->index;
    stack_size = stacksize(stack);
    largest = stack->index;
    while (stack_size > 0)
    {
        stack = rotate_a(stack, NULL);
        if (stack->index > largest)
            largest = stack->index;
        stack_size--;
    }
    while (stack->index != tmp)
        stack = rotate_a(stack, NULL);
    return (largest);
}

//Function to get the largest index in the current stack
//Rewrite these to not use rotate
int get_smallest_index(t_stack *stack)
{
    int stack_size;
    int smallest;
    int tmp;

    tmp = stack->index;
    stack_size = stacksize(stack);
    smallest = stack->index;
    while (stack_size > 0)
    {
        stack = rotate_a(stack, NULL);
        if (stack->index < smallest)
            smallest = stack->index;
        stack_size--;
    }
    while (stack->index != tmp)
        stack = rotate_a(stack, NULL);
    return (smallest);
}

//Function to initialize vars struct
t_vars3 *init_vars_3(t_stack **a, t_vars3 *vars)
{
    int i;

    i = 0;
    vars = (t_vars3*)malloc(sizeof(t_vars3));

    //Init stack size
    vars->stack_size_a = stacksize(*a);
    //Init chunk_sizes and index values
    while (i < 20)
    {
        vars->chunk_size[i] = 0;
        vars->chunk_start[i] = -1;//Init to -1 because 0 is a valid index
        vars->chunk_end[i] = -1; //Init to -1 for consistency
        i++;
    }
    //Init keep_count to 0;
    vars->keep_count = 0;
    
    //Init keep indices to -1
    i = 0;
    while (i < 500)
    {
        vars->keep_indices[i] = -1;
        i++;
    }
    return (vars);
}

//Function to initialize chunk information
t_vars3 *init_chunk_info(t_vars3 *vars, int chunk_count)
{
    int i;
    int size_tot;

    i = 0;
    size_tot = 0;
    vars->chunk_count = chunk_count; //Copy chunk count to struct
  	//Assign chunk sizes based on stack size
	while (i < chunk_count)
	{
		vars->chunk_size[i] = (vars->stack_size_a / chunk_count);
		i++;
	}
	//Add remainder (if any) to the final chunk.
	vars->chunk_size[chunk_count - 1] = vars->chunk_size[chunk_count - 1] + (vars->stack_size_a % chunk_count);

    //Use chunk sizes to assign start and end index values for each chunk.
	i = 0;
	while (i < chunk_count)
	{
		//Start index is equal to previous total size
		vars->chunk_start[i] = size_tot;
		//Set new running total
		size_tot = size_tot + vars->chunk_size[i];
		//End index is equal to running total of all chunk sizes minus one
		vars->chunk_end[i] = size_tot - 1;
		i++;
	}
	return (vars);
}

//Function to store all the indices to be kept in stack a
void    chunk_keep_indices(t_stack **a, t_vars3 *vars)
{
    int top_index;
    int tmp_index;
    int i;

    i = 0;

    //Find the starting index
    vars->start_index = find_start(*a);

    //Store current top index to rotate back later
    top_index = (*a)->index;

    //Rotate to start index
    while ((*a)->index != vars->start_index)
        *a = rotate_a(*a, NULL);

    //Add start index to array, set tmp index, and rotate once
    vars->keep_indices[i] = (*a)->index;
    tmp_index = (*a)->index;
    *a = rotate_a(*a, NULL);

    //Rotate until we return to the start index, adding to array
    while ((*a)->index != vars->start_index)
    {
        if ((*a)->index > tmp_index)
        {
            i++;//Preincrement i to save a line in function earlier on
            vars->keep_indices[i] = (*a)->index;
            tmp_index = (*a)->index;
        }
        *a = rotate_a(*a, NULL);
    }
    //Set keep_count (equal to i plus one, as i is tracking indices not count)
    vars->keep_count = i + 1; 
    //Rotate back to initial top index
    while ((*a)->index != top_index)
        *a = rotate_a(*a, NULL);
}

//Function to push all values except keep_indices values to stack b in specific order
void    chunk_push_remain(t_stack **a, t_stack **b, int **actions, t_vars3 *vars)
{
    int chunk_remain;
    int chunk_x;//This will always be the larger value chunk
    int chunk_y;//This will always be the smaller value chunk
    int count_x;
    int count_y;

    int i;

    i = 0;

    //Set remaining number of chunks to be pushed
    chunk_remain = vars->chunk_count;
    //Find the first chunk(s) to be pushed. Single chunk is chunk_count is odd

    //Run loop until all chunks have been push
    while (chunk_remain > 0)
    {
        //Set the current chunks and their size(s) (make sure to subtact any keep_indices from counts)
        //If odd number of chunks
        if ((vars->chunk_count % 2) == 1)
        {
            chunk_x = (vars->chunk_count / 2); // Do not need to add one because we are 0 indexed 
            chunk_y = -1;
            count_x = get_chunk_count(chunk_x, vars);//Need to get count less any indices that are being kept
            count_y = 0;
        }
        else
        {
            chunk_x = (vars->chunk_count / 2) + i; //Add i to get proper chunk
            chunk_y = (vars->chunk_count / 2) - 1 - i; //Subtract one as we are 0 indexed for chunks, and add i to get to proper chunk
            count_x = get_chunk_count(chunk_x, vars);//Need to get count less any indices that are being kept
            count_y = get_chunk_count(chunk_y, vars);//Need to get count less any indices that are being kept
        }

        //Reset stack size for each iteration of inner loop
        vars->stack_size_a = stacksize(*a);
        //Run loop until entire stack has been traversed once
        while (vars->stack_size_a > 0)
        {
            //If all values in current chunk(s) have been pushed, exit loop to save actions
            if (count_x == 0 && count_y == 0)
                break;
            //If in current chunk x and not in keep list, push and decrement count
            if (((*a)->index >= vars->chunk_start[chunk_x] && (*a)->index <= vars->chunk_end[chunk_x]) && !check_keep((*a)->index, vars))
            {
                push_b(b, a, actions);
                count_x--;
                vars->stack_size_a--; //Decrement stack size to control loop
            }
            //If in current chunk y and not in keep list, push, rotate b once to push to bottom, and decrement count
            else if (((*a)->index >= vars->chunk_start[chunk_y] && (*a)->index <= vars->chunk_end[chunk_y]) && !check_keep((*a)->index, vars) && chunk_y != -1)
            {
                push_b(b, a, actions);
                *b = rotate_b(*b, actions);
                count_y--;
                vars->stack_size_a--; //Decrement stack size to control loop
            }
            //If not pushed, rotate once
            else
            {
                *a = rotate_a(*a, actions);
                vars->stack_size_a--; //Decrement stack size to control loop
            }
        }
        //If chunk_remain is odd, decrement it by one
        if ((chunk_remain % 2) == 1)
            chunk_remain = chunk_remain - 1;
        //Else decrement by 2
        else
            chunk_remain = chunk_remain - 2;
        i++; //Increment i to control which chunks are selected
    }

}

//Function to check if current index is in the keep_indices array 
int check_keep(int index, t_vars3 *vars)
{
    int i;

    i = 0;
    while (vars->keep_indices[i] != -1)
    {
        //Return 1 if the index passed is matched in the keep_indices array
        if (index == vars->keep_indices[i])
            return (1);
        i++;
    }
    //Return 0 if no match found
    return (0);
}

//Function to count number of indices in a chunk less the number of keep_indices in that chunk
int get_chunk_count(int chunk, t_vars3 *vars)
{
    int count;
    int i;
    
    count = vars->chunk_size[chunk];
    i = 0;
    //Iterate through keep_indices and decrement count if and values are found that belong to current chunk
    while (vars->keep_indices[i] != -1)
    {
        if ((vars->keep_indices[i] >= vars->chunk_start[chunk]) && (vars->keep_indices[i] <= vars->chunk_end[chunk]))
            count--;
        i++;
    }
    return (count);
}
    

/*

1. Find the largest number of elements that can be kept already in ascdending order

2. Push the rest of the elements to stack b in chunks
    - Can push 2 chunks at a time by using rotate to move to the back of stack, or just pushing to top of stack
    - Want to push in descending order, and maintain order, so start with the middle chunks and move out
    - WIth 8 chunks, we would push chunk 4 and 5, then 3 and 6, then 2 and 7 and finally 1 and 8
    - The lower of the 2 chunks would be pushed and then rotate the stack once to push it to the bottom
    - The upper chunk just gets pushed
    - Try to make the number of chunks adjustable for optimization. If the number of chunks is odd, 
        push the middle chunk first, then do double pushing.
3. See if it is worth applying swaps?
4. Push values back to a in order, being mindful of the location of the existing elements in stack a.


*/
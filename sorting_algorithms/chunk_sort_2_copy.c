/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_sort_2_copy.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 13:34:47 by jgoad             #+#    #+#             */
/*   Updated: 2021/12/15 18:25:06 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_push_swap.h"



//Controller function
void    chunk_sort_new(t_stack **a, t_stack **b, int **actions)
{
    t_vars3 *vars;
    int chunk_count;

    chunk_count = 18;//Odd numbered chunk counts are currently broken. Need to find ideal chunk counts. Can dynamically set which chunk size to use based on stack size

    //Initialize vars and set chunk information
    vars = NULL;
    vars = init_vars_3(a, vars); //May not need this function, can malloc right here
    init_chunk_info(vars, chunk_count);

    //Find the list of elements to be kept in stack a
    chunk_keep_indices(a, vars);

    //Push the remaining elements to stack b in chunks 
    chunk_push_remain(a, b, actions, vars);
    /*
    //Push using closest distance algorithm
    while (*b)
    {
        chunk_find_closest(*a, *b, vars);
        chunk_push_match(a, b, vars, actions);
    }
    */

    //Push elements from b back to a in order
    chunk_push_order(a, b, actions, vars);

    //Rotate a back to index 0 on top (replace with a seperate fun)
	chunk_rotate_to_target(a, 0, actions, 'a');
}



//Function to rotate stacks until there is a match on top of each
void	chunk_push_match(t_stack **a, t_stack **b, t_vars3 *vars, int **actions)
{
	//Rotate stacks until target index is on top of each stack
	while (((*a)->index != vars->target_index_a) || ((*b)->index != vars->target_index_b))
	{
		//Check if we can do a double rotate action
		if ((chunk_find_rotate_direction(*a, vars->target_index_a) == 1) && (chunk_find_rotate_direction(*b, vars->target_index_b) == 1))
			rotate_both(a, b, actions);
		//Check if we can do a double rev rotate action
		else if ((chunk_find_rotate_direction(*a, vars->target_index_a) == -1) && (chunk_find_rotate_direction(*b, vars->target_index_b) == -1))
			rev_rotate_both(a, b, actions);
		//Otherwise run single actions
		else
		{
			//Run action for a
			if (chunk_find_rotate_direction(*a, vars->target_index_a) == -1)
				*a = rev_rotate_a(*a, actions);
			else if (chunk_find_rotate_direction(*a, vars->target_index_a) == 1)
				*a = rotate_a(*a, actions);
			//run action for b
			if (chunk_find_rotate_direction(*b, vars->target_index_b) == -1)
				*b = rev_rotate_b(*b, actions);
			else if (chunk_find_rotate_direction(*b, vars->target_index_b) == 1)
				*b = rotate_b(*b, actions);
		}
	}
	//Push from b to a once match is in position
	if ((*b)->index < (*a)->index)
		push_a(a, b, actions);
	//If b->index is larger than top index of a, rotate a once before pushing
	else if ((*b)->index > (*a)->index)
	{
		*a = rotate_a(*a, actions);
		push_a(a, b, actions);
	}
}

//Function to push elements back into stack a in the proper order
//This goes in order of b top to bottom, how to integrate with closest match optimization?
void    chunk_push_order(t_stack **a, t_stack **b, int **actions, t_vars3 *vars)
{
    int chunk_remain;
    int i;
    int chunk_current;
    int smallest;


    chunk_remain = vars->chunk_count; //Set total number of chunks
    i = 0;
    //Rotate stack a so the largest value is on top. 
    while (vars->keep_indices[i] != -1)
        i++;
    chunk_rotate_to_target(a, vars->keep_indices[i - 1], actions, 'a');
   
    //While there are any elements left in b
    while (*b && chunk_remain > 0)
    {
        chunk_current = chunk_remain - 1; // Set current chunk index (count - 1)
        smallest = get_smallest_index(*a); //Find the smallest index in stack a
        //Check that there are still elements in the current chunk left in stack b. If not decrement to next chunk and break
        if (get_largest_index(*b) < vars->chunk_start[chunk_current])
            chunk_remain--;
        //If there is a member of current chunk in stack a and in stack b
        else if (get_largest_index(*a) >= vars->chunk_start[chunk_current] && get_largest_index(*b) >= vars->chunk_start[chunk_current])
        {
            chunk_find_closest(*a, *b, vars, chunk_current); //Find the closest match within the current chunk.
            //IF NO TARGEST ARE FOUND, TARGET INDEXES ARE 0 AND INFINITE LOOP OCCURS. Need so search for an insertion range as opposed to single element?
            //Rotate till position is correct
            while ((*a)->index != vars->target_index_a || (*b)->index != vars->target_index_b)
            {
                //Check double reverse rotate
                if (chunk_find_rotate_direction(*a, vars->target_index_a) == -1 && chunk_find_rotate_direction(*b, vars->target_index_b) == -1)
                    rev_rotate_both(a, b, actions);
                //Check double rotate
                else if (chunk_find_rotate_direction(*a, vars->target_index_a) == -1 && chunk_find_rotate_direction(*b, vars->target_index_b) == -1)
                    rotate_both(a, b, actions);
                else
                {
                    //Single actions for stack a
                    if (chunk_find_rotate_direction(*a, vars->target_index_a) == -1)
                        *a = rev_rotate_a(*a, actions);
                    else if (chunk_find_rotate_direction(*a, vars->target_index_a) == 1)
                        *a = rotate_a(*a, actions);
                    //Single actions for stack b
                    if (chunk_find_rotate_direction(*b, vars->target_index_b) == -1)
                        *b = rev_rotate_b(*b, actions);
                    else if (chunk_find_rotate_direction(*b, vars->target_index_b) == 1)
                        *b = rotate_b(*b, actions);
                }
            }
            //Push element if b is smaller than a
            if ((*b)->index < (*a)->index)
                push_a(a, b, actions);
            //Otherwise rotate and push. We are rev rotating when not necessary to setup previous step
            else
            {
                *a = rotate_a(*a, actions);
                push_a(a, b, actions);
            }
        }
        //If no member of current chunk in stack a, push chunk below highest value (which means make sure the smallest value is on top)
        else
        {
            if (chunk_find_rotate_direction(*a, smallest) == -1)
            {
                while ((*a)->index != smallest)
                    *a = rev_rotate_a(*a, actions);
            }
            else if (chunk_find_rotate_direction(*a, smallest) == 1)
            {
                while ((*a)->index != smallest)
                    *a = rotate_a(*a, actions);
            }
            push_a(a, b, actions);
        }
    }
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
    int chunk_x; //This will always be the larger value chunk
    int chunk_y; //This will always be the smaller value chunk
    int count_x; //Amount of elements remaining in chunk x
    int count_y; //Amount of elements remaining in chunk y
    int i;

    i = 0;

    //Set remaining number of chunks to be pushed
    chunk_remain = vars->chunk_count;

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

//Function to find the closest element within the current chunk to move based on relative positions of stack a and stack b
//If no mat
void	chunk_find_closest(t_stack *a, t_stack *b, t_vars3 *vars, int chunk)
{
    int chunk_count_b;
    int chunk_count_a;
    int top_a;
    int top_b;
    t_stack *tmp_high;
    t_stack *tmp_low;

    chunk_count_b = chunk_count_elements(b, chunk, vars);
	vars->stack_size_a = stacksize(a);
	vars->stack_size_b = stacksize(b);
	vars->count_b = 0;
	vars->distance = 1000; //Arbitary max distance to start with
    vars->distance_b = 0;
    vars->distance_a = 0;
    top_a = a->index;
    top_b = b->index;

	//Rotate through members of current chunk in b
	while (chunk_count_b > 0)
	{
        //Need to move next element of current chunk in stack b. Is this doing anything useful?
        while (b->index < vars->chunk_start[chunk] || b->index > vars->chunk_end[chunk])
        {
            b = rotate_b(b, NULL);
            vars->count_b++;
        }
        chunk_count_b--; //Decrement count of elements in stack b to iterate through
        
        //Iterate through all elements in stack a and then return to initial location. DO I NEED THIS ANYMORE?
        chunk_count_a = chunk_count_elements(a, chunk, vars);
        vars->count_a = 0;

        int i = 0;
        //If there is an element of the current chunk in A.
    
            while (i < vars->stack_size_a) //IF THERE IS NO MATCH FOR CURRENT CHUNK IN STACK A, NO TARGETS ARE SET. MAYBE JUST ITERATE THROUGH WHOLE STACK AND LET CONDITIONAL STATEMENTS FIND THE CLOSEST?
            {
                /*
                //Move to next element of current chunk in stack a. THIS ASSUMES THERE IS A MEMBER OF CURRENT CHUNK IN STACK A.
                while (a->index < vars->chunk_start[chunk] || a->index > vars->chunk_end[chunk])
                {
                    a = rotate_a(a, NULL);
                    vars->count_a++;
                }
                tmp_high = a->next;
                tmp_low = stacklast(a);
                chunk_count_a--; //Decrement count of elements in stack a to iterate through
                */
                //Check if these two elements are a match(a is in order but b is not, so we only care about stack a positioning).
                //Need to make sure we arent overlapping the largest and smallest chunks with these checks
                //THis control needs to be changed. We want to find a valid insertion position not an adjacent match
                //Valid insertion position means the index to be pushed from stack b is smaller than a->next->index but larger than a->index

                tmp_high = a->next;
                tmp_low = stacklast(a);

                //If b is larger than a and it is also larger than the largest index in a
                //If b is smaller than a and it also smaller than the smallest index in a
                //If b is largeer than a but smaller then a->next
                //If b is smaller than a but larger than a->previous
                if ((b->index > a->index && b->index > get_largest_index(a)) || (b->index < a->index && b->index < get_smallest_index(a) && a->index == get_smallest_index(a)) || (b->index > a->index && b->index < tmp_high->index) || (b->index < a->index && b->index > tmp_low->index))      
                {
                    //Get absolute distance of index from top of each stack
                    if (vars->count_a > (vars->stack_size_a / 2))
                        vars->distance_a = vars->stack_size_a - vars->count_a; //If the current element is past the midpoint of stack, distance is taken in reverse (stack_size minus current distance)
                    if (vars->count_b > (vars->stack_size_b / 2))
                        vars->distance_b = vars->stack_size_b - vars->count_b;
                    //If distance is shorter, update distance and target indexes
                    if ((vars->distance_a + vars->distance_b) < vars->distance)
                    {
                        vars->distance = vars->distance_a + vars->distance_b;
                        vars->target_index_a = a->index;
                        vars->target_index_b = b->index;
                    }
                }
                //Rotate a and increment i and increment distance (count_a)
                i++;
                vars->count_a++;
                a = rotate_a(a, NULL);
            }
        //Return stack a to original starting index
        while (a->index != top_a)
            a = rotate_a(a, NULL);
	}
    //Return stack b to original starting index
     while (b->index != top_b)
            b = rotate_b(b, NULL);
}

//Function to get number of elements of a chunk in the current stack
int chunk_count_elements(t_stack *stack, int chunk, t_vars3 *vars)
{
    int count;
    t_stack *tmp;

    count = 0;
    tmp = stack;
    //Iterate through entire stack
    while (tmp)
    {
        //If current element is a member of target chunk increment count
        if (tmp->index >= vars->chunk_start[chunk] && tmp->index <= vars->chunk_end[chunk])
            count++;
        tmp = tmp->next;
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

/*

//Function to rotate stacks until there is a match on top of each
void	chunk_push_match(t_stack **a, t_stack **b, t_vars3 *vars, int **actions)
{
	//Rotate stacks until target index is on top of each stack
	while (((*a)->index != vars->target_index_a) || ((*b)->index != vars->target_index_b))
	{
		//Check if we can do a double rotate action
		if ((chunk_find_rotate_direction(*a, vars->target_index_a) == 1) && (chunk_find_rotate_direction(*b, vars->target_index_b) == 1))
			rotate_both(a, b, actions);
		//Check if we can do a double rev rotate action
		else if ((chunk_find_rotate_direction(*a, vars->target_index_a) == -1) && (chunk_find_rotate_direction(*b, vars->target_index_b) == -1))
			rev_rotate_both(a, b, actions);
		//Otherwise run single actions
		else
		{
			//Run action for a
			if (chunk_find_rotate_direction(*a, vars->target_index_a) == -1)
				*a = rev_rotate_a(*a, actions);
			else if (chunk_find_rotate_direction(*a, vars->target_index_a) == 1)
				*a = rotate_a(*a, actions);
			//run action for b
			if (chunk_find_rotate_direction(*b, vars->target_index_b) == -1)
				*b = rev_rotate_b(*b, actions);
			else if (chunk_find_rotate_direction(*b, vars->target_index_b) == 1)
				*b = rotate_b(*b, actions);
		}
	}
	//Push from b to a once match is in position
	if ((*b)->index < (*a)->index)
		push_a(a, b, actions);
	//If b->index is larger than top index of a, rotate a once before pushing
	else if ((*b)->index > (*a)->index)
	{
		*a = rotate_a(*a, actions);
		push_a(a, b, actions);
	}
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

//Function to find the closest element to move based on relative positions of stack a and stack b
//Ensure that both stacks always return to start positions
//Prob need further optimization, there will not always be an adjacent element match in the stack. Switch logic to finding a range match and check if lowest or highest element?
void	chunk_find_closest(t_stack *a, t_stack *b, t_vars3 *vars)
{
	vars->stack_size_a = stacksize(a);
	vars->stack_size_b = stacksize(b);
	vars->count_b = 0;
	vars->distance = 1000; //Arbitary max distance to start with
	//Rotate completely through b
	while (vars->count_b < vars->stack_size_b)
	{
		vars->count_a = 0; //Reset count for stack a before starting a new loop
		vars->distance_b = vars->count_b; //Set the current distance from the top of stack b
		while (vars->count_a < vars->stack_size_a) //Rotate completely through stack a
		{
			vars->distance_a = vars->count_a; //Set the current distance from the top of stack a
			if ((a->index == b->index + 1) || (a->index == b->index - 1)) //If there is an index match AND this match has a shorter distance, update target indexes and distance
			{
				//Get absolute distance of index from top of each stack
				if (vars->count_a > (vars->stack_size_a / 2))
					vars->distance_a = vars->stack_size_a - vars->count_a; //If the current element is past the midpoint of stack, distance is taken in reverse (stack_size minus current distance)
				if (vars->count_b > (vars->stack_size_b / 2))
					vars->distance_b = vars->stack_size_b - vars->count_b;
				//If distance is shorter, update distance and target indexes
				if ((vars->distance_b + vars->distance_a) < vars->distance)
				{
					vars->distance = vars->distance_a + vars->distance_b;
					vars->target_index_a = a->index;
					vars->target_index_b = b->index;
				}
			}
			//Rotate stack a and increment count
			a = rotate_a(a, NULL);
			vars->count_a++;
		}
		//Rotate b once and then run through a again
		b = rotate_b(b, NULL);
		vars->count_b++;
	}
}

*/
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ordered_pairs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 14:15:45 by jgoad             #+#    #+#             */
/*   Updated: 2021/11/28 14:42:56 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ORDER PAIRS ALGORITHM

1. Create pairs with adjacent index values, pushing half of values to stack b

2. Run swap operations until pairs are in the correct order.

3. Push integers back to stack a in the correct order


Optimization:
	- Run double swap and double rotate when possible
	- Try and push pairs to b in ascending order.


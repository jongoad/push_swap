/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/15 15:43:06 by jgoad             #+#    #+#             */
/*   Updated: 2021/09/20 14:52:02 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	string_count(char *s, char c)
{
	unsigned int	count;
	unsigned int	found;

	count = 0;
	found = 0;
	while (*s)
	{
		while (*s == c && *s)
		{
			found = 0;
			s++;
		}
		while (*s != c && *s)
		{
			if (found == 0)
				count++;
			found = 1;
			s++;
		}
	}
	return (count);
}

static unsigned int	char_count(char const *s, char c)
{
	unsigned int	count;

	count = 0;
	while ((*s != c) && *s)
	{
		count++;
		s++;
	}
	return (count);
}

static char	*get_string(char const *s, char c)
{
	char			*str;
	unsigned int	i;

	i = 0;
	str = malloc(sizeof(char) * char_count(s, c) + 1);
	if (!str)
		return (NULL);
	while ((*s != c) && *s)
	{
		str[i] = *s;
		i++;
		s++;
	}
	str[i] = '\0';
	return (str);
}

static void	free_array(char **array, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	int		count;
	int		i;

	i = -1;
	if (!s)
		return (NULL);
	count = string_count((char *)s, c);
	array = malloc(sizeof(char *) * (count) + 1);
	if (!array)
		return (NULL);
	array[count] = NULL;
	while (++i < count)
	{
		while (*s == c)
			s++;
		array[i] = get_string((char *)s, c);
		if (!array[i])
		{
			free_array(array, i);
			return (NULL);
		}
		s += ft_strlen(array[i]);
	}
	return (array);
}

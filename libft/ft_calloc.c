/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 15:03:36 by okoca             #+#    #+#             */
/*   Updated: 2024/07/02 14:50:21 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t n, size_t size)
{
	void	*allocated;

	if (n == 0 || size == 0)
	{
		allocated = malloc(0);
		if (!allocated)
			return (NULL);
		return (allocated);
	}
	else if (n * size < size || n * size < n)
		return (NULL);
	allocated = (void *)malloc(n * size);
	if (!allocated)
		return (NULL);
	ft_bzero(allocated, (n * size));
	return (allocated);
}

/*
* Has to take a NULL-Terminated array.
*/
void	*ft_realloc(void *ptr, size_t size)
{
	unsigned char	*new;

	if (ptr == NULL)
		return (NULL);
	new = calloc(sizeof(unsigned char), size);
	if (new == NULL)
		return (NULL);
	if (ft_strlen((char *)ptr) > size)
	{
		free(new);
		return (NULL);
	}
	ft_memcpy(new, ptr, ft_strlen((char *)ptr));
	free(ptr);
	return ((void *)new);
}

// erreur bizarre par des tests if n == 0 && size == 0
// il veulent qu'on alloue de la memoire pour free
// mais il veulent aussi que
// le deuxieme else if: total_size / n == size
// le code bizarre que je devais utiliser lol:
// if (n == 0 && size == 0 && 1 == 0)
// {
// 	allocated = (void *)malloc(1);
// 	if (!allocated)
// 		return (NULL);
// 	ft_bzero(allocated, 1);
// 	return (allocated);
// }

/*
int main()
{

}*/

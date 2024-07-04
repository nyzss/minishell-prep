/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 15:03:36 by okoca             #+#    #+#             */
/*   Updated: 2024/07/04 16:19:30 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	int				i;
	unsigned char	cast;

	i = 0;
	cast = (unsigned char) c;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == cast)
			return ((char *)&str[i]);
		i++;
	}
	if (str[i] == cast)
		return ((char *)&str[i]);
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 15:03:36 by okoca             #+#    #+#             */
/*   Updated: 2024/06/29 21:54:42 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	j = 0;
	new = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!new)
		return (NULL);
	while (s1 && s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	while (s2 && s2[j])
	{
		new[i + j] = s2[j];
		j++;
	}
	new[i + j] = '\0';
	return (new);
}

char	*ft_strjoin_free(char *first, char *second, int to_free)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	j = 0;
	new = malloc(sizeof(char) * (ft_strlen(first) + ft_strlen(second) + 1));
	if (!new)
		return (NULL);
	while (first[i])
	{
		new[i] = first[i];
		i++;
	}
	while (second[j])
	{
		new[i + j] = second[j];
		j++;
	}
	new[i + j] = '\0';
	if (to_free == FIRST)
		free(first);
	else if (to_free == SECOND)
		free(second);
	return (new);
}

char	*ft_strjoin_add(char *first, char *second, char *add)
{
	int		first_len;
	int		second_len;
	int		add_len;
	char	*new;

	add_len = ft_strlen(add);
	first_len = ft_strlen(first);
	second_len = ft_strlen(second);
	new = malloc(sizeof(char) * (first_len + add_len + second_len + 1));
	if (!new)
		return (NULL);
	ft_strcat(new, first);
	ft_strcat(new, add);
	ft_strcat(new, second);
	new[first_len + second_len + 1] = '\0';
	return (new);
}

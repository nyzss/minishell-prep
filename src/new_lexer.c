/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_lexer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 08:41:47 by okoca             #+#    #+#             */
/*   Updated: 2024/07/05 10:25:15 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

// int	lex_count_string(char *str)
// {

// }

void	lexer(char *str)
{
	int	i;
	int	count;
	int	len;

	i = 0;
	count = 0;
	len = ft_strlen(str);
	while (str[i])
	{
		if (str[i] == SingleQuote || str[i] == DoubleQuote)
		{
			printf("token: [");
			count++;
			// i++;
			while (str[i])
			{
				printf("%c", str[i]);
				if ((str[i] == SingleQuote || str[i] == DoubleQuote) && str[i + 1] == ' ')
				{
					i++;
					break ;
				}
				i++;
			}
			printf("]\n");
		}
		else if (str[i] == PipeChar)
		{
			printf("token: [%c]\n", str[i]);
			count++;
			i++;
		}
		else if (str[i] == InfileChar)
		{
			printf("token: [%c", str[i]);
			if (str[i + 1] == InfileChar)
			{
				printf("%c]\n", str[i]);
				count++;
				i += 2;
			}
			else
			{
				printf("]\n");
				i++;
				count++;
			}
		}
		else if (str[i] == OutfileChar)
		{
			printf("token: [%c", str[i]);
			if (str[i + 1] == OutfileChar)
			{
				printf("%c]\n", str[i]);
				count++;
				i += 2;
			}
			else
			{
				printf("]\n");
				i++;
				count++;
			}
		}
		else if (str[i] != SpaceChar)
		{
			count++;
			printf("token: [");
			while (str[i] && str[i] != SpaceChar)
			{
				printf("%c", str[i]);
				i++;
			}
			printf("]\n");
		}
		else
			i++;
	}
}

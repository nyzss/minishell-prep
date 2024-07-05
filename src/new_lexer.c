/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_lexer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 08:41:47 by okoca             #+#    #+#             */
/*   Updated: 2024/07/05 10:46:27 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

int	lex_is_meta(char c)
{
	int		i;
	char	*meta;

	i = 0;
	meta = ft_strdup("<>|\'\"");
	while (meta[i])
	{
		if (meta[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	lex_count_string(char *str)
{
	int	i;

	i = 0;
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
	return (i);
}

int	lex_count_redirection(char *str, t_meta_char meta)
{
	int		i;
	char	meta_char;

	i = 0;
	meta_char = meta;
	if (str[i + 1] == meta_char)
	{
		printf("%c]\n", str[i]);
		i += 2;
	}
	else
	{
		printf("]\n");
		i++;
	}
	return (i);
}

int	lex_count_raw(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != SpaceChar)
	{
		if (lex_is_meta(str[i]))
			break ;
		printf("%c", str[i]);
		i++;
	}
	return (i);
}

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
			i += lex_count_string(&(str[i]));
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
			count++;
			i += lex_count_redirection(&(str[i]), InfileChar);
		}
		else if (str[i] == OutfileChar)
		{
			printf("token: [%c", str[i]);
			i += lex_count_redirection(&(str[i]), OutfileChar);
			count++;
		}
		else if (str[i] != SpaceChar)
		{
			count++;
			printf("token: [");
			i += lex_count_raw(&(str[i]));
			printf("]\n");
		}
		else
			i++;
	}
}

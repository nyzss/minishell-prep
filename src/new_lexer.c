/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_lexer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 08:41:47 by okoca             #+#    #+#             */
/*   Updated: 2024/07/08 09:02:12 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

int	lex_is_meta(char c)
{
	int		i;
	char	*meta;
	int		found;

	i = 0;
	found = 0;
	meta = ft_strdup("()&<>|\'\"");
	while (meta[i])
	{
		if (meta[i] == c)
			found = i;
		i++;
	}
	free(meta);
	return (found);
}

int	lex_count_string(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] == SingleQuote || str[i] == DoubleQuote)
			&& (str[i + 1] == SpaceChar || str[i + 1] == ParenthesisOpen
			|| str[i + 1] == ParenthesisClose || str[i + 1] == PipeChar
			|| str[i + 1] == InfileChar || str[i + 1] == OutfileChar
			|| str[i + 1] == AndChar))
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
		i += 2;
	else
		i++;
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
		i++;
	}
	return (i);
}

void	*lex_clear_tokens(t_token *token)
{
	t_token	*tmp;

	while (token != NULL)
	{
		tmp = token;
		if (token->value)
			free(token->value);
		token = token->next_token;
		free(tmp);
	}
	return (NULL);
}

t_token	*lexer(char *str)
{
	int	i;
	int	len;
	t_token	*token;
	t_token	*tmp;

	i = 0;
	len = 0;
	token = NULL;
	while (str[i])
	{
		if (str[i] == SingleQuote || str[i] == DoubleQuote)
		{
			tmp = create_token(RawString, ft_strndup(&(str[i]), lex_count_string(&(str[i]))));
			printf("lex_count_string %d\n", lex_count_string(&(str[i])));
			add_token(&token, tmp);
			i += ft_strlen(tmp->value);
		}
		else if (str[i] == OrChar && str[i + 1] == OrChar)
		{
			tmp = create_token(Or, ft_strndup(&(str[i]), 2));
			add_token(&token, tmp);
			i += ft_strlen(tmp->value);
		}
		else if (str[i] == PipeChar)
		{
			tmp = create_token(Pipe, ft_strndup(&(str[i]), 1));
			add_token(&token, tmp);
			i += ft_strlen(tmp->value);
		}
		else if (str[i] == ParenthesisOpen)
		{
			tmp = create_token(GroupOpen, ft_strndup(&(str[i]), 1));
			add_token(&token, tmp);
			i += ft_strlen(tmp->value);
		}
		else if (str[i] == ParenthesisClose)
		{
			tmp = create_token(GroupClose, ft_strndup(&(str[i]), 1));
			add_token(&token, tmp);
			i += ft_strlen(tmp->value);
		}
		else if (str[i] == AndChar && str[i + 1] == AndChar)
		{
			tmp = create_token(And, ft_strndup(&(str[i]), 2));
			add_token(&token, tmp);
			i += ft_strlen(tmp->value);
		}
		else if (str[i] == InfileChar)
		{
			len = lex_count_redirection(&(str[i]), InfileChar);
			if (len == 1)
				tmp = create_token(Infile, ft_strndup(&(str[i]), len));
			else
				tmp = create_token(HereDoc, ft_strndup(&(str[i]), len));
			add_token(&token, tmp);
			i += ft_strlen(tmp->value);
		}
		else if (str[i] == OutfileChar)
		{
			len = lex_count_redirection(&(str[i]), OutfileChar);
			if (len == 1)
				tmp = create_token(Outfile, ft_strndup(&(str[i]), len));
			else
				tmp = create_token(Append, ft_strndup(&(str[i]), len));
			add_token(&token, tmp);
			i += ft_strlen(tmp->value);
		}
		else if (str[i] != SpaceChar)
		{
			tmp = create_token(RawString, ft_strndup(&(str[i]), lex_count_raw(&(str[i]))));
			add_token(&token, tmp);
			i += ft_strlen(tmp->value);
		}
		else
			i++;
	}
	return (token);
}

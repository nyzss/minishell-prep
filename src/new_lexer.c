/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_lexer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 08:41:47 by okoca             #+#    #+#             */
/*   Updated: 2024/07/05 14:21:50 by okoca            ###   ########.fr       */
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
	free(meta);
	return (0);
}

int	lex_count_string(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
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

void	lex_clear_tokens(t_token *token)
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
			add_token(&token, tmp);
			i += ft_strlen(tmp->value);
		}
		else if (str[i] == PipeChar)
		{
			tmp = create_token(Pipe, ft_strndup(&(str[i]), 1));
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

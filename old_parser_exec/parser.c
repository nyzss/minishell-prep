/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 09:00:00 by okoca             #+#    #+#             */
/*   Updated: 2024/07/06 19:09:34 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

int	check_non_next(t_token *token, t_token_type type)
{
	if (token->type == type)
	{
		if (token->next_token == NULL)
			return (1);
		if (token->next_token->type != DoubleQuoteString
			&& token->next_token->type != SingleQuoteString
			&& token->next_token->type != RawString)
			return (1);
	}
	return (0);
}

char	**split_once(char *value)
{
	char	**arg;
	int		i;
	int		j;
	int		s1_len;
	int		s2_len;
	char	*s1;
	char	*s2;

	i = 0;
	j = 0;
	s1_len = 0;
	s2_len = 0;
	arg = malloc(sizeof(char *) * 2);
	while (value[s1_len] && value[s1_len] != ' ')
		s1_len++;
	while (value[s1_len + s2_len])
		s2_len++;
	if (s2_len == 0)
		return (NULL);
	s1 = malloc(sizeof(char) * (s1_len + 1));
	s2 = malloc(sizeof(char) * (s2_len + 1));
	while (value[i])
	{
		if (i < s1_len)
			s1[i] = value[i];
		else if (i != s1_len && i < s1_len + s2_len)
		{
			s2[j] = value[i];
			j++;
		}
		i++;
	}
	s2_len--;
	s1[s1_len] = '\0';
	s2[s2_len] = '\0';
	printf("s1_len: %d - s2_len: %d\n", s1_len, s2_len);
	arg[0] = s1;
	arg[1] = s2;
	return (arg);
}


// this takes in a token with a raw string which should normally be
// a filename and a command
// if the line is $ < Makefile cat
// then in this function we get the "Makefile cat" part
void	split_filename_command(t_token *token)
{
	char	**arg;
	t_token	*new_token;

	arg = split_once(token->value);
	if (arg == NULL)
		return ;
	// raw string for now but should be command next time
	new_token = create_token(RawString, arg[1]);
	new_token->next_token = token->next_token;
	token->next_token = new_token;
	token->value = arg[0];
}

void	handle_quoted_args(t_token *token)
{
	t_token	*tmp;

	tmp = token->next_token;
	while (tmp != NULL &&
		(tmp->type == DoubleQuoteString
		|| tmp->type == SingleQuoteString))
	{
		// concat every next strings into the value
		// printf("TMPTMPTMP val: %s\n", tmp->value);
		token->value = ft_strjoin(token->value, " ");
		token->value = ft_strjoin(token->value, tmp->value);
		tmp = tmp->next_token;
	}
	token->next_token = tmp;
}

int	token_checker(t_token *token)
{
	int	error_code;
	t_token	*tmp;

	tmp = token;
	error_code = 0;
	while (tmp != NULL)
	{
		error_code = check_non_next(tmp, Outfile);
		error_code = check_non_next(tmp, Infile);
		error_code = check_non_next(tmp, Append);
		error_code = check_non_next(tmp, HereDoc);
		error_code = check_non_next(tmp, Pipe);
		tmp = tmp->next_token;
	}
	if (error_code == 0)
	{
		while (token != NULL)
		{
			if (token->type == Outfile || token-> type == Infile
				|| token->type == Append || token->type == HereDoc)
			{
				if (token->next_token->type == RawString)
					split_filename_command(token->next_token);
				token->next_token->type = Filename;
			}
			if (token->type == RawString)
			{
				// handle_quoted_args(token);
				token->type = Command;
			}
			token = token->next_token;
		}
	}
	return (error_code);
}

int	handle_arguments(t_token *token)
{
	while (token!= NULL)
	{
		if (token->type == RawString)
			token->type = Argument;
		else
			break ;
		token = token->next_token;
	}
	return (0);
}

int	new_token_checker(t_token *token)
{
	int	error_code;
	t_token	*tmp;

	tmp = token;
	error_code = 0;
	while (tmp != NULL)
	{
		error_code = check_non_next(tmp, Outfile);
		error_code = check_non_next(tmp, Infile);
		error_code = check_non_next(tmp, Append);
		error_code = check_non_next(tmp, HereDoc);
		error_code = check_non_next(tmp, Pipe);
		tmp = tmp->next_token;
	}
	if (error_code == 0)
	{
		while (token != NULL)
		{
			if (token->type == Outfile || token-> type == Infile
				|| token->type == Append || token->type == HereDoc)
			{
				if (token->next_token->type == RawString)
					split_filename_command(token->next_token);
				token->next_token->type = Filename;
			}
			else if (token->type == RawString)
			{
				// handle_quoted_args(token);
				token->type = Command;
				if (token->next_token != NULL)
					handle_arguments(token->next_token);
			}
			else if (token->type == SingleQuoteString || token->type == DoubleQuoteString)
				handle_arguments(token->next_token);
			token = token->next_token;
		}
	}
	// while
	return (error_code);
}

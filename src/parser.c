/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 09:00:00 by okoca             #+#    #+#             */
/*   Updated: 2024/06/28 10:31:02 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

// int	check_redirection(t_token *token)
// {
// 	if (token->type == Infile || token->type == Outfile)
// 	{
// 		if (token->next_token == NULL)
// 			return (1);
// 		if (token->next_token->type != DoubleQuoteString
// 			&& token->next_token->type != SingleQuoteString
// 			&& token->next_token->type != RawString)
// 			return (1);
// 	}
// 	return (0);
// }

// int	check_pipes(t_token *token)
// {
// 	if (token->type == Pipe)
// 	{
// 		if (token->next_token == NULL)
// 			return (1);
// 		if (token->next_token->type != DoubleQuoteString
// 			&& token->next_token->type != SingleQuoteString
// 			&& token->next_token->type != RawString)
// 			return (1);
// 	}
// }

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
	{
		printf("GOES IN HERE %s\n", token->value);
		return ;
	}
	// raw string for now but should be command next time
	new_token = create_token(RawString, arg[1], 0);
	new_token->next_token = token->next_token;
	token->next_token = new_token;
	token->value = arg[0];
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
			token = token->next_token;
		}
	}
	return (error_code);
}
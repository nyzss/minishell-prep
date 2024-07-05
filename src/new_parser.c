/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:25:31 by okoca             #+#    #+#             */
/*   Updated: 2024/07/05 15:39:20 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

int	ps_is_redir(t_token *token)
{
	if (token->type == Infile || token->type == Outfile
		|| token->type == Append || token->type == HereDoc)
		return (1);
	return (0);
}

int	ps_check_redir(t_token *token)
{
	while (token != NULL)
	{
		if (ps_is_redir(token) == 1)
		{
			if (!token->next_token || token->next_token->type != RawString)
				return (PARSING_ERROR);
		}
		token = token->next_token;
	}
	return (0);
}

int	ps_rename_types(t_token *token)
{
	t_token	*tmp;
	int		command;

	command = 0;
	tmp = token;
	while (token != NULL)
	{
		if (ps_is_redir(token))
			token->next_token->type = Filename;
		token = token->next_token;
	}
	token = tmp;
	while (token != NULL)
	{
		if (token->type == Pipe)
			command = 0;
		if (token->type == RawString && command == 0)
		{
			token->type = Command;
			command = 1;
		}
		else if (token->type == RawString)
			token->type = Argument;
		token = token->next_token;
	}
	return (0);
}

int	ps_handle_expand(t_token *token)
{
	while (token != NULL)
	{
		if (token->type == RawString)
		{
			token->value = ps_handle_quotes(token->value);
			if (token->value == NULL)
				return (PARSING_ERROR);
		}
		token = token->next_token;
	}
	return (0);
}

int	parser(t_token *token)
{
	if (ps_check_redir(token) == PARSING_ERROR)
		return (PARSING_ERROR);
	ps_handle_expand(token);
	ps_rename_types(token);
	return (0);
}
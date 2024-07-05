/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:25:31 by okoca             #+#    #+#             */
/*   Updated: 2024/07/05 14:36:45 by okoca            ###   ########.fr       */
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

	tmp = token;
	while (token != NULL)
	{
		if (ps_is_redir(token))
			token->next_token->type = Filename;
		token = token->next_token;
	}
}

int	parser(t_token *token)
{
	if (ps_check_redir(token) == PARSING_ERROR)
		return (PARSING_ERROR);
	ps_rename_types(token);
	return (0);
}
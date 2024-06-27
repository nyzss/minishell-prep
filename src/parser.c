/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 09:00:00 by okoca             #+#    #+#             */
/*   Updated: 2024/06/27 09:12:05 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

int	check_redirection(t_token *token)
{
	if (token->type == Infile || token->type == Outfile)
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

int	token_checker(t_token *token)
{
	int	error_code;

	error_code = 0;
	while (token != NULL)
	{
		error_code = check_redirection(token);
		token = token->next_token;
	}
	return (error_code);
}
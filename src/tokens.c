/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 19:07:35 by okoca             #+#    #+#             */
/*   Updated: 2024/07/07 09:29:30 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

int	print_token(t_token *token)
{
	while (token != NULL)
	{
		if (token->type == Infile)
			printf("(Infile)");
		else if (token->type == Outfile)
			printf("(Outfile)");
		else if (token->type == Pipe)
			printf("(Pipe)");
		else if (token->type == DoubleQuoteString)
			printf("(DoubleQuoteString)");
		else if (token->type == SingleQuoteString)
			printf("(SingleQuoteString)");
		else if (token->type == RawString)
			printf("(RawString)");
		else if (token->type == Command)
			printf("(Command)");
		else if (token->type == HereDoc)
			printf("(HereDoc)");
		else if (token->type == Append)
			printf("(Append)");
		else if (token->type == Or)
			printf("(Or)");
		else if (token->type == And)
			printf("(And)");
		else if (token->type == Filename)
			printf("(Filename)");
		else if (token->type == GroupOpen)
			printf("(GroupOpen)");
		else if (token->type == GroupClose)
			printf("(GroupClose)");
		else if (token->type == Argument)
			printf("(Argument)");
		printf(" - value: [%s]\n", token->value);
		token = token->next_token;
	}
	printf("\n");
	return (0);
}

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next_token = NULL;
	return (token);
}

t_token	*last_token(t_token *head)
{
	while (head->next_token != NULL)
		head = head->next_token;
	return (head);
}

int	add_token(t_token **head, t_token *new)
{
	if (new == NULL)
		return (1);
	if (*head == NULL)
		*head = new;
	else
		last_token((*head))->next_token = new;
	return (0);
}

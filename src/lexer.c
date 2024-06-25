/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 07:52:55 by okoca             #+#    #+#             */
/*   Updated: 2024/06/25 08:25:16 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

int	print_token(t_token *token)
{
	while (token != NULL)
	{
		if (token->type == SingleQuote || token->type == DoubleQuote || token->type == Infile || token->type == Outfile)
		{
			if (token->type == SingleQuote)
				printf("(SingleQuote)");
			else if (token->type == DoubleQuote)
				printf("(DoubleQuote)");
			else if (token->type == Infile)
				printf("(Infile)");
			else if (token->type == Outfile)
				printf("(Outfile)");
			printf(" - index: %d", token->index);
			printf(" - value: %c\n", *(token->value));
		}
		else
		{
			if (token->type == String)
				printf("(String)");
			printf(" - index: %d", token->index);
			printf(" - value: %s\n", token->value);
		}
		token = token->next_token;
	}
	return (0);
}

t_token	*create_token(t_token_type type, char *value, int index)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->index = index;
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

// single quote by default
char	*create_string(char *str, t_token_type rec_type, int *index)
{
	int		len;
	int		i;
	char	quote_type;
	char	*new;

	i = 0;
	len = 0;
	quote_type = '\"';
	if (rec_type == SingleQuote)
		quote_type = '\'';
	while (str[len] && str[len] != quote_type)
		len++;
	new = malloc(sizeof(char) * (len + 1));
	while (i < len)
	{
		new[i] = str[i];
		i++;
	}
	new[i] = '\0';
	*index += i;
	return (new);
}

t_token	*tokenize_line(char *buf)
{
	t_token	*head;
	t_token	*tmp;
	int		i;

	i = 0;
	head = NULL;
	while (buf[i])
	{
		if (buf[i] == '\'')
		{
			tmp = create_token(SingleQuote, &(buf[i]), i);
			add_token(&head, tmp);
			i++;
			add_token(&head, create_token(String, create_string(&(buf[i]), SingleQuote, &i), i));
		}
		else if (buf[i] == '\"')
		{
			tmp = create_token(SingleQuote, &(buf[i]), i);
			add_token(&head, tmp);
			i++;
			add_token(&head, create_token(String, create_string(&(buf[i]), DoubleQuote, &i), i));
		}
		else if (buf[i] == '<')
		{
			tmp = create_token(Infile, &(buf[i]), i);
			add_token(&head, tmp);
		}
		else if (buf[i] == '>')
		{
			tmp = create_token(Outfile, &(buf[i]), i);
			add_token(&head, tmp);
		}
		i++;
	}
	return (head);
}

int	clear_token(t_token **token)
{
	t_token	*next;
	t_token	*tmp;

	next = (*token);
	while (next != NULL)
	{
		tmp = next;
		next = next->next_token;
		free(tmp);
	}
	*token = NULL;
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 07:52:55 by okoca             #+#    #+#             */
/*   Updated: 2024/06/25 10:45:17 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

int	print_token(t_token *token)
{
	while (token != NULL)
	{
		if (token->type == SingleQuote || token->type == DoubleQuote
			|| token->type == Infile || token->type == Outfile
			|| token->type == Pipe)
		{
			if (token->type == SingleQuote)
				printf("(SingleQuote)");
			else if (token->type == DoubleQuote)
				printf("(DoubleQuote)");
			else if (token->type == Infile)
				printf("(Infile)");
			else if (token->type == Outfile)
				printf("(Outfile)");
			else if (token->type == Pipe)
				printf("(Pipe)");
			printf(" - index: \"%d\"", token->index);
			printf(" - value: \"%c\"\n", *(token->value));
		}
		else
		{
			if (token->type == String)
				printf("(String)");
			else if (token->type == Command)
				printf("(Command)");
			printf(" - index: \"%d\"", token->index);
			printf(" - value: \"%s\"\n", token->value);
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
	*index += i + 1;
	return (new);
}

int	check_if_meta(char c)
{
	if (c == '\'' || c == '\"' || c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

t_token *create_command(char *str, int *index)
{
	int		i;
	int		len;
	char	*new;
	t_token	*new_token;

	i = 0;
	len = 0;
	while (str[len] && check_if_meta(str[len]) == 0)
		len++;
	new = malloc(sizeof(char) * (len + 1));
	while (i < len)
	{
		new[i] = str[i];
		i++;
	}
	new[i] = '\0';
	new_token = create_token(Command, new, *index);
	*index += i;
	*index -= 1;
	return (new_token);
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
			add_token(&head, create_token(String, create_string(&(buf[i + 1]), SingleQuote, &i), i));
		}
		else if (buf[i] == '\"')
		{
			tmp = create_token(SingleQuote, &(buf[i]), i);
			add_token(&head, tmp);
			add_token(&head, create_token(String, create_string(&(buf[i + 1]), DoubleQuote, &i), i));
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
		else if (buf[i] == '|')
		{
			tmp = create_token(Pipe, &(buf[i]), i);
			add_token(&head, tmp);
		}
		else if (buf[i] != ' ')
		{
			tmp = create_command(&(buf[i]), &i);
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
		if (tmp->type == String || tmp->type == Command)
			free(tmp->value);
		free(tmp);
	}
	*token = NULL;
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 07:52:55 by okoca             #+#    #+#             */
/*   Updated: 2024/07/05 14:23:54 by okoca            ###   ########.fr       */
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
		else if (token->type == Filename)
			printf("(Filename)");
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

char	*create_string(char *str, t_token_type rec_type, int *index)
{
	int		len;
	int		i;
	char	quote_type;
	char	*new;
	// int		new_len;

	i = 0;
	len = 0;
	// new_len = 0;
	quote_type = '\"';
	if (rec_type == SingleQuoteString)
		quote_type = '\'';
	while (str[len] && str[len] != quote_type)
		len++;
	new = malloc(sizeof(char) * (len + 1));
	if (!new)
		return (NULL);
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
	char	*tmp;
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
	tmp = ft_strtrim(new, " ");
	free(new);
	new_token = create_token(RawString, tmp);
	*index += i - 1;
	// *index -= 1;
	return (new_token);
}

t_token *new_create_command(char *str, int *index)
{
	int		i;
	int		len;
	char	*new;
	char	*tmp;
	t_token	*new_token;

	i = 0;
	len = 0;
	while (str[len] && check_if_meta(str[len]) == 0 && str[len] != ' ')
		len++;
	new = malloc(sizeof(char) * (len + 1));
	while (i < len)
	{
		new[i] = str[i];
		i++;
	}
	new[i] = '\0';
	tmp = ft_strtrim(new, " ");
	free(new);
	new_token = create_token(RawString, tmp);
	handle_env_expand(new_token);
	*index += i - 1;
	// *index -= 1;
	return (new_token);
}

int		get_len_str(int len, char *str)
{
	int	i;
	int	j;
	int	new_len;

	i = 0;
	j = 0;
	new_len = 0;
	while (i < len)
	{
		if (str[i] == '\"')
		{
			i++;
			while (i < len && str[i] != '\"')
			{
				new_len++;
				i++;
			}
		}
		else if (str[i] == '\'')
		{
			i++;
			while (i < len && str[i] != '\'')
			{
				new_len++;
				i++;
			}
		}
		else
			new_len++;
		i++;
	}
	return (new_len);
}

int	new_get_len_str(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && (str[i + 1] == ' '))
			break;
		i++;
	}
	return (i);
}
char	*new_ft_strcat(char *dest, char *src)
{
	int		i;
	int		dst_len;

	i = 0;
	dst_len = ft_strlen(dest);
	while (src[i] != '\0')
	{
		dest[dst_len + i] = src[i];
		i++;
	}
	dest[dst_len + i] = '\0';
	return (dest);
}

char	*newer_create_string(char *str, int *index)
{
	t_token	*tmp_token;
	int		i;
	int		j;
	int		len;
	char	*tmp;
	char	*new;

	i = 0;
	tmp_token = NULL;
	tmp = NULL;
	new = NULL;
	len = new_get_len_str(str);
	while (i < len)
	{
		j = 1;
		if (str[i] == '\'')
		{
			while (i + j < len && str[i + j] != '\'')
				j++;
			tmp = ft_strndup(&(str[i + 1]), j - 1);
			add_token(&(tmp_token), create_token(SingleQuoteString, tmp));
			i += j;
		}
		else if (str[i] == '\"')
		{
			while (i + j < len && str[i + j] != '\"')
				j++;
			tmp = ft_strndup(&(str[i + 1]), j - 1);
			add_token(&(tmp_token), create_token(DoubleQuoteString, tmp));
			i += j;
		}
		else
		{
			while (i + j < len && str[i + j] != '\"' && str[i + j] != '\'')
				j++;
			tmp = ft_strndup(&(str[i]), j);
			add_token(&(tmp_token), create_token(RawString, tmp));
			i += j - 1;
		}
		i++;
	}
	*index += i - 1;
	handle_env_expand(tmp_token);
	new = combine_tokens(tmp_token);
	return (new);
}

t_token	*new_tokenizer(char *buf)
{
	t_token	*head;
	t_token	*tmp;
	int		i;

	i = 0;
	head = NULL;
	while (buf[i])
	{
		if (buf[i] == '\'' || buf[i] == '\"')
		{
			tmp = create_token(RawString, newer_create_string(&(buf[i]), &i));
			add_token(&head, tmp);
			// printf("hello");
		}
		else if (buf[i] == '<')
		{
			if (buf[i + 1] != '\0' && buf[i + 1] == '<')
			{
				tmp = create_token(HereDoc, "<<");
				add_token(&head, tmp);
				i++;
			}
			else
			{
				tmp = create_token(Infile, &(buf[i]));
				add_token(&head, tmp);
			}
		}
		else if (buf[i] == '>')
		{
			if (buf[i + 1] != '\0' && buf[i + 1] == '>')
			{
				tmp = create_token(Append, ">>");
				add_token(&head, tmp);
				i++;
			}
			else
			{
				tmp = create_token(Outfile, &(buf[i]));
				add_token(&head, tmp);
			}
		}
		else if (buf[i] == '|')
		{
			tmp = create_token(Pipe, &(buf[i]));
			add_token(&head, tmp);
		}
		else if (buf[i] != ' ')
		{
			tmp = new_create_command(&(buf[i]), &i);
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
		if (tmp->type == SingleQuoteString || tmp->type == DoubleQuoteString || tmp->type == RawString || tmp->type == Command || tmp->type == Filename)
			free(tmp->value);
		free(tmp);
	}
	*token = NULL;
	return (0);
}

// echo """"'hello world'"""" lol

// 'hello world'

// ''hello world''

// "" -> \0

// echo "'"""'hello world'"""'" lol

// remove all quotes from string already before anything
// would be a lot more safe in terms of allocations
// while (str[i])
// {
// 	j = 0;
// 	if (str[i] == '\"')
// 	{
// 		while (str[i + j + 1] != '\"')
// 		{
// 			len++;
// 			j++;
// 		}
// 		i += j + 1;
// 	}
// 	else if (str[i] == '\'')
// 	{
// 		while (str[i + j + 1] != '\'')
// 		{
// 			len++;
// 			j++;
// 		}
// 		i += j + 1;
// 	}
// 	else
// 		len++;
// 	i++;
// }

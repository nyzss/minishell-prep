/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 11:22:20 by okoca             #+#    #+#             */
/*   Updated: 2024/06/24 22:01:38 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

int	print_token(t_token *token)
{
	while (token != NULL)
	{
		if (token->type == SingleQuote)
			printf("(SingleQuote)");
		else if (token->type == DoubleQuote)
			printf("(DoubleQuote)");
		else if (token->type == String)
			printf("(String)");
		else if (token->type == Infile)
			printf("(Infile<)");
		else if (token->type == Outfile)
			printf("(Outfile>)");
		printf(" - index: %d", token->index);
		printf(" - value: %c\n", *(token->value));
		token = token->next_token;
	}
	return (0);
}

int	m_child(char *buf, char **env)
{
	pid_t	pid;
	char	**args;
	char	*path;

	args = ft_split(buf, ' ');
	if (!args)
		return (2);
	path = p_get_path(args[0], env);
	if (access(path, F_OK | X_OK) != 0)
	{
		if (path)
			free(path);
		p_cleanup_array(args);
		return (1);
	}
	pid = fork();
	if (pid == 0)
		p_exec(path, args, env);
	waitpid(pid, NULL, 0);
	return (0);
}

void	handle_sigint(int status)
{
	(void)status;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
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
		}
		else if (buf[i] == '\"')
		{
			tmp = create_token(SingleQuote, &(buf[i]), i);
			add_token(&head, tmp);
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

int	main(int ac, char **av, char **env)
{
	char	*buf;
	t_token	*token;

	(void)ac;
	(void)av;
	token = NULL;
	while (1)
	{
		buf = readline("\033[1;33mlexer_prep -$ \033[0m");
		if (strncmp(buf, "exit", 4) == 0)
		{
			free(buf);
			break ;
		}
		signal(SIGINT, handle_sigint);
		token = tokenize_line(buf);
		print_token(token);
		if (m_child(buf, env) != 0)
		{
			printf("Command not found!\n");
		}
		printf("%s\n", buf);
		if (buf)
			free(buf);
		clear_token(&token);
	}
	printf("exiting shell...\n");
	return (0);
}
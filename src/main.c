/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 11:22:20 by okoca             #+#    #+#             */
/*   Updated: 2024/06/26 13:52:11 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

int	m_child(char *buf, char **env)
{
	char	**args;
	char	*path;

	args = ft_split(buf, ' ');
	if (!args)
		return (2);
	path = p_get_path(args[0]);
	if (access(path, F_OK | X_OK) != 0)
	{
		if (path)
			free(path);
		p_cleanup_array(args);
		return (1);
	}
	p_exec(path, args, env);
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

// int	check_token(t_token *token)
// {
// 	int	single_quote_nb;
// 	int	double_quote_nb;

// 	single_quote_nb = 0;
// 	double_quote_nb = 0;
// 	while (token != NULL)
// 	{
// 		if (token->type == SingleQuoteString)
// 			single_quote_nb++;
// 		else if (token->type == DoubleQuoteString)
// 			double_quote_nb++;
// 		token = token->next_token;
// 	}
// 	if (single_quote_nb % 2 != 0 || double_quote_nb % 2 != 0)
// 		return (1);
// 	return (0);
// }

t_token	*get_next_command(t_token *head)
{
	while (head != NULL && head->next_token != NULL)
	{
		if (head->next_token->type == Command)
			return (head->next_token);
		head = head->next_token;
	}
	return (NULL);
}

int	call_command(char *path, char **env, int last)
{
	pid_t	pid;
	int		fds[2];

	pipe(fds);
	pid = fork();
	if (pid == 0)
	{
		close(fds[0]);
		if (last == 0)
			dup2(fds[1], STDOUT_FILENO);
		m_child(path, env);
		exit(1);
	}
	else
	{
		if (last == 0)
			dup2(fds[0], STDIN_FILENO);
	}
	close(fds[0]);
	close(fds[1]);
	return (0);
}

int	count_commands(t_token *token)
{
	int	count;

	count = 0;
	while (token != NULL)
	{
		if (token->type == Command)
			count++;
		token = token->next_token;
	}
	return (count);
}

// write from parent [1]
// read from child [0]
void	handle_execution(t_token *token, char **env)
{
	int		i;
	int		count;
	int		total_command;
	int		last;

	i = 0;
	last = 0;
	count = 0;
	total_command = count_commands(token);
	if (total_command > 0)
	{
		while (token != NULL && token->type == Command)
		{
			if (count == total_command - 1)
				last = 1;
			call_command(token->value, env, last);
			token = get_next_command(token);
			count++;
		}
		while (i < total_command)
		{
			wait(NULL);
			i++;
		}
		// while (i < total_command)
		// {
		// 	printf("pids: %d\n", pids[i]);
		// 	waitpid(pids[i], NULL, 0);
		// 	i++;
		// }
		// waitpid(pid, NULL, 0);
	}
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
		signal(SIGINT, handle_sigint);
		buf = readline("\033[1;33mlexer_prep -$ \033[0m");
		if (buf <= 0)
			break ;
		if (strncmp(buf, "exit", 4) == 0)
		{
			free(buf);
			break ;
		}
		token = tokenize_line(buf);
		print_token(token);
		handle_execution(token, env);
		printf("%s\n", buf);
		if (buf)
			free(buf);
		clear_token(&token);
	}
	printf("exiting shell...\n");
	return (0);
}
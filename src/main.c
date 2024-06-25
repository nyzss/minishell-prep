/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 11:22:20 by okoca             #+#    #+#             */
/*   Updated: 2024/06/25 14:29:06 by okoca            ###   ########.fr       */
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
	signal_n = 1;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	check_token(t_token *token)
{
	int	single_quote_nb;
	int	double_quote_nb;

	single_quote_nb = 0;
	double_quote_nb = 0;
	while (token != NULL)
	{
		if (token->type == SingleQuote)
			single_quote_nb++;
		else if (token->type == DoubleQuote)
			double_quote_nb++;
		token = token->next_token;
	}
	if (single_quote_nb % 2 != 0 || double_quote_nb % 2 != 0)
		return (1);
	return (0);
}

t_token	*get_next_command(t_token *head)
{
	while (head != NULL)
	{
		if (head->type == Command)
			return (head);
		head = head->next_token;
	}
	return (NULL);
}

// write from parent [1]
// read from child [0]
void	handle_execution(t_token *token, char **env)
{
	pid_t	pid;
	// pid_t	pid2;
	int		fds[2];

	pipe(fds);
	while (token != NULL && token->type == Command)
	{
		pid = fork();
		if (pid == 0)
		{
			dup2(fds[1], STDOUT_FILENO);
			dup2(fds[0], STDIN_FILENO);
			close(fds[0]);
			close(fds[1]);
			if (m_child(token->value, env) != 0)
			{
				printf("Command not found!\n");
				exit(1);
			}
		}
		token = get_next_command(token);
		close(fds[0]);
		close(fds[1]);
		// wait(NULL);
		// waitpid(pid2, NULL, 0);
		if (signal_n == 1)
			kill(pid, SIGKILL);
		waitpid(pid, NULL, 0);
	}
}

// pid2 = fork();
// if (pid2 == 0)
// {
// 	dup2(fds[0], STDIN_FILENO);
// 	close(fds[1]);
// 	close(fds[0]);
// 	m_child(token->next_token->next_token->value, env);
// 	if (m_child(token->next_token->next_token->value, env) != 0)
// 	{
// 		printf("Command not found!\n");
// 		exit(1);
// 	}
// }

int	main(int ac, char **av, char **env)
{
	char	*buf;
	t_token	*token;

	(void)ac;
	(void)av;
	(void)env;
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
		if (check_token(token) == 1)
			printf("error: unclosed quotes\n");
		else
		{
			print_token(token);
			handle_execution(token, env);
		}
		printf("%s\n", buf);
		if (buf)
			free(buf);
		clear_token(&token);
	}
	printf("exiting shell...\n");
	return (0);
}
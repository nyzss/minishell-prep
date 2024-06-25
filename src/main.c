/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 11:22:20 by okoca             #+#    #+#             */
/*   Updated: 2024/06/25 09:53:20 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

int	m_child(char *buf, char **env)
{
	pid_t	pid;
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
		if (check_token(token) == 1)
			printf("error: unclosed quotes\n");
		else
		{
			print_token(token);
			if (m_child(buf, env) != 0)
			{
				printf("Command not found!\n");
			}
		}
		printf("%s\n", buf);
		if (buf)
			free(buf);
		clear_token(&token);
	}
	printf("exiting shell...\n");
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 11:22:20 by okoca             #+#    #+#             */
/*   Updated: 2024/06/24 17:32:58 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

int	print_token(t_token token)
{
	if (token.type == SingleQuote)
		printf("(SingleQuote)");
	else if (token.type == DoubleQuote)
		printf("(DoubleQuote)");
	else if (token.type == String)
		printf("(String)");
	printf(" - index: %d", token.index);
	printf(" - value: %c\n", *(token.value));
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
		p_exec(path, args);
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

// t_token	*add_token(, int index)
// {

// }

int	main(int ac, char **av, char **env)
{
	char	*buf;
	int		i;
	// t_token	*token;

	(void)ac;
	(void)av;
	while (1)
	{
		i = 0;
		buf = readline("\033[1;33mlexer_prep -$ \033[0m");
		if (strncmp(buf, "exit", 4) == 0)
		{
			free(buf);
			break ;
		}
		signal(SIGINT, handle_sigint);
		if (m_child(buf, env) != 0)
		{
			printf("Command not found!\n");
		}
		// while (buf[i])
		// {
		// 	if (buf[i] == '\'')
		// 	{
		// 		token.type = SingleQuote;
		// 		token.index = i;
		// 		token.value = &(buf[i]);
		// 		print_token(token);
		// 	}
		// 	else if (buf[i] == '\"')
		// 	{
		// 		token.type = DoubleQuote;
		// 		token.index = i;
		// 		token.value = &(buf[i]);
		// 		print_token(token);
		// 	}
		// 	i++;
		// }
		printf("%s\n", buf);
		free(buf);
	}
	printf("exiting shell...\n");
	return (0);
}
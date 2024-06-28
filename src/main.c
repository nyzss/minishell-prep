/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 11:22:20 by okoca             #+#    #+#             */
/*   Updated: 2024/06/28 18:31:24 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

char	**combine_args(t_cmd *cmd, char **args)
{
	int		i;
	char	**new;

	i = 0;
	while (args[i])
		i++;
	new = malloc(sizeof(char *) * (cmd->arg_nb + i + 1));
	// make new list with both of them
}

int	m_child(t_cmd *cmds, char **env)
{
	char	**args;
	char	*path;

	args = ft_split(cmds->value, ' ');
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
		if (head->next_token->type == RawString)
			return (head->next_token);
		head = head->next_token;
	}
	return (NULL);
}

int	call_command(t_cmd *cmds, t_exec *exec, int last)
{
	pid_t	pid;
	int		fds[2];

	pipe(fds);
	pid = fork();
	if (pid == 0)
	{
		if (last == 0)
			dup2(fds[1], STDOUT_FILENO);
		else if (exec->outfile_fd != STDOUT_FILENO)
			dup2(exec->outfile_fd, STDOUT_FILENO);
		close(fds[0]);
		close(fds[1]);
		m_child(cmds, exec->env);
	}
	else
	{
		// if (last == 0)
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
		if (token->type == RawString)
			count++;
		token = token->next_token;
	}
	return (count);
}

int	main(int ac, char **av, char **env)
{
	char	*buf;
	t_token	*token;
	t_exec	*exec;
	char	*prompt = ESCAPE_F COLOR_YELLOW ESCAPE_S "prep -$ " ESCAPE_F COLOR_RESET ESCAPE_S;

	(void)ac;
	(void)av;
	token = NULL;
	while (1)
	{
		signal(SIGINT, handle_sigint);
		buf = readline(prompt);
		if (buf == 0)
			break ;
		if (strncmp(buf, "exit", 4) == 0)
		{
			free(buf);
			break ;
		}
		token = tokenize_line(buf);
		if (token_checker(token) != 0)
			printf("nuh uh\n");
		else
		{
			print_token(token);
			exec = build_exec(token, env);
			do_exec(exec);
		}
		print_exec(exec);
		printf("\ninput: \"%s\"\n", buf);
		if (buf)
			free(buf);
		clear_token(&token);
	}
	printf("exiting shell...\n");
	return (0);
}
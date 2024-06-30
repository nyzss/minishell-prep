/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 11:22:20 by okoca             #+#    #+#             */
/*   Updated: 2024/06/30 16:38:25 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

char	**combine_args(t_cmd *cmd, char **args)
{
	int		i;
	int		j;
	char	**new;
	t_args	*extra_args;

	i = 0;
	j = 0;
	extra_args = cmd->extra_args;
	while (args[i])
		i++;
	new = malloc(sizeof(char *) * (cmd->arg_count + i + 1));
	while (args[j])
	{
		new[j] = args[j];
		j++;
	}
	i = 0;
	while (extra_args != NULL && i < cmd->arg_count)
	{
		new[j + i] = extra_args->value;
		extra_args = extra_args->next_arg;
		i++;
	}
	new[j + i] = NULL;
	return (new);
}

int	m_child(t_cmd *cmds, char **env)
{
	char	**args;
	char	*path;

	args = ft_split(cmds->value, ' ');
	if (!args)
		return (2);
	args = combine_args(cmds, args);
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
		{
			dup2(exec->outfile_fd, STDOUT_FILENO);
			close(exec->outfile_fd);
		}
		close(fds[0]);
		close(fds[1]);
		m_child(cmds, exec->env);
	}
	else
	{
		dup2(fds[0], STDIN_FILENO);
	}
	close(fds[0]);
	close(fds[1]);
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

int	check_line(char *line)
{
	if (line[0] == '\0')
		return (1);
	return (0);
}

int	handle_loop(char *buf, char **env)
{
	t_token	*token;
	t_exec	*exec;
	HISTORY_STATE *state;
	t_pipe	*pipes;

	token = NULL;
	exec = NULL;
	if (strncmp(buf, "exit", 4) == 0)
	{
		free(buf);
		return (1);
	}
	token = tokenize_line(buf);
	if (token_checker(token) != 0)
		printf("nuh uh\n");
	else
	{
		handle_env_expand(token);

		pipes = build_pipe(token, env);
		// (void)pipes;
		print_pipe(pipes);

		exec = build_exec(token, env);
		do_exec(exec);
	}
	add_history(buf);
	state = history_get_history_state();
	#if DEBUG
	print_token(token);
	print_exec(exec);
	printf("\ninput: \"%s\"\n", buf);
	print_history(state);
	#endif
	clear_token(&token);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	char	*buf;
	char	*prompt = ESCAPE_F COLOR_YELLOW_A ESCAPE_S "prep -$ " ESCAPE_F COLOR_RESET ESCAPE_S;

	(void)ac;
	(void)av;
	using_history();
	signal(SIGINT, handle_sigint);
	while (1)
	{
		buf = readline(prompt);
		if (buf == 0)
			break ;
		if (check_line(buf) == 0)
		{
			if (handle_loop(buf, env) != 0)
				break ;
		}
		free(buf);
	}
	printf("exiting shell...\n");
	return (0);
}

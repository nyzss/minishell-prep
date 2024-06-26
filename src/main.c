/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 11:22:20 by okoca             #+#    #+#             */
/*   Updated: 2024/07/01 22:41:28 by okoca            ###   ########.fr       */
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

char	**list_to_args(t_cmd *cmd)
{
	int		i;
	char	**args;
	t_args	*l_args;
	int		total_args;

	i = 0;
	total_args = cmd->arg_count + 1;
	l_args = cmd->extra_args;
	args = malloc(sizeof(char *) * (total_args + 1));
	args[0] = cmd->value;
	i++;
	while (l_args != NULL)
	{
		args[i] = l_args->value;
		l_args = l_args->next_arg;
		i++;
	}
	args[i] = NULL;
	return (args);
}

int	m_child(t_cmd *cmds, char **env)
{
	char	**args;
	char	*path;
	int		status;

	if (handle_built_in(cmds, &status) == 0)
	{
		path = p_get_path(cmds->value);
		if (access(path, F_OK | X_OK) != 0)
		{
			return (1);
		}
		args = list_to_args(cmds);
		p_exec(path, args, env);
	}
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
	t_pipe	*pipes;
	int		status;

	(void)env;
	token = NULL;
	pipes = NULL;
	status = 0;
	// token = tokenize_line(buf);
	token = new_tokenizer(buf);
	if (new_token_checker(token) != 0)
	{
		printf("nuh uh\n");
		free(buf);
		return (0);
	}
	else
	{
		handle_env_expand(token);

		pipes = build_pipe(token, env);
		status = do_pipes(pipes);
	}
	add_history(buf);
	#if DEBUG
	print_token(token);
	print_pipe(pipes);
	printf("\ninput: \"%s\"\n", buf);
	#endif
	clear_token(&token);
	if (status == SHOULD_EXIT)
	{
		free(buf);
		return (status);
	}
	return (0);
}

int	main(int ac, char **av, char **env)
{
	char	*buf;
	char	*prompt = ESCAPE_F COLOR_YELLOW_A ESCAPE_S "prep -$ " ESCAPE_F COLOR_RESET ESCAPE_S;

	(void)ac;
	(void)av;
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

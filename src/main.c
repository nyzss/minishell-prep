/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 11:22:20 by okoca             #+#    #+#             */
/*   Updated: 2024/07/03 10:02:53by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

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

int	m_child(t_ctx *ctx, t_cmd *cmds, char **env)
{
	char	**args;
	char	*path;
	int		status;

	if (handle_built_in(ctx, cmds, &status) == 0)
	{
		path = p_get_path(cmds->value);
		if (access(path, F_OK | X_OK) != 0)
		{
			return (1);
		}
		args = list_to_args(cmds);
		p_exec(path, args, env);
	}
	else
		exit(status);
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

void	handle_sigquit(int status)
{
	(void)status;
	// printf("\n");
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
	if (line == NULL)
		return (1);
	else if (line[0] == '\0')
		return (1);
	return (0);
}

int	handle_loop(t_ctx *ctx)
{
	t_pipe	*pipes;
	int		status;

	pipes = NULL;
	status = 0;
	ctx->token = new_tokenizer(ctx->line);
	// print_token(ctx->token);
	if (new_token_checker(ctx->token) != 0)
	{
		printf("nuh uh\n");
		return (0);
	}
	else
	{
		// print_token(ctx->token);
		//new step to handle command arguments

		pipes = build_pipe(ctx->token);
		ctx->pipes = pipes;
		status = do_pipes(ctx);
	}
	add_history(ctx->line);
	#if DEBUG
	// print_token(ctx->token);
	// print_pipe(pipes);
	// printf("\ninput: \"%s\"\n", ctx->line);
	#endif
	clear_token(&(ctx->token));
	if (status == SHOULD_EXIT)
	{
		free(ctx->line);
		return (status);
	}
	return (0);
}

void	get_stds(t_ctx *ctx)
{
	ctx->def_in = dup(STDIN_FILENO);
	ctx->def_out = dup(STDOUT_FILENO);
}

void	reset_stds(t_ctx *ctx)
{
	dup2(ctx->def_in, STDIN_FILENO);
	dup2(ctx->def_out, STDOUT_FILENO);
}

void	handle_signals()
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}

int	main(int ac, char **av, char **env)
{
	char	*prompt = ESCAPE_F COLOR_YELLOW_A ESCAPE_S "prep -$ " ESCAPE_F COLOR_RESET ESCAPE_S;
	t_ctx	ctx;
	// char	*t = "Hello world! $PATH $envkl $lolz HELLOWORLD!!!!!!! $MANPAGER $LESS $PAGER $PWD";

	// expand_env(&t);
	// printf("%s\n", t);
	(void)ac;
	(void)av;
	ctx.line = NULL;
	ctx.pipes = NULL;
	ctx.token = NULL;
	ctx.env = env;
	get_stds(&ctx);
	handle_signals();
	while (1)
	{
		ctx.line = readline(prompt);
		if (ctx.line == NULL)
			break ;
		if (check_line(ctx.line) == 0)
		{
			lexer(ctx.line);
			// if (handle_loop(&ctx) != 0)
			// 	break ;
		}
		free(ctx.line);
		ctx.line = NULL;
	}
	close(ctx.def_in);
	close(ctx.def_out);
	printf("exiting shell...\n");
	return (0);
}

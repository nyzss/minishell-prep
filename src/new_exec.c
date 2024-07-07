/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 19:03:59 by okoca             #+#    #+#             */
/*   Updated: 2024/07/07 16:26:58 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

t_pipe	*new_build_pipe(t_token **tokens)
{
	t_pipe	*pipe_tab;
	t_token	*token;

	token = (*tokens);
	pipe_tab = malloc(sizeof(t_pipe));
	pipe_tab->cmd = NULL;
	pipe_tab->args = NULL;
	pipe_tab->nb_args = 0;
	pipe_tab->filename = NULL;
	pipe_tab->in_fd = STDIN_FILENO;
	pipe_tab->out_fd = STDOUT_FILENO;
	pipe_tab->next = NULL;
	while (token != NULL)
	{
		if (token->type == Infile)
			add_filename(&(pipe_tab->filename), create_filename(token->next_token->value, token->type));
		else if (token->type == Outfile)
			add_filename(&(pipe_tab->filename), create_filename(token->next_token->value, token->type));
		else if (token->type == HereDoc)
			add_filename(&(pipe_tab->filename), create_filename(token->next_token->value, token->type));
		else if (token->type == Append)
			add_filename(&(pipe_tab->filename), create_filename(token->next_token->value, token->type));
		else if (token->type == Command)
			pipe_tab->cmd = token->value;
		else if (token->type == Argument)
			add_arg(&(pipe_tab->args), create_args(token->value));
		else if (token->type == Pipe)
		{
			pipe_tab->next = new_build_pipe(&(token->next_token));
			break ;
		}
		else if (token->type != Filename)
			break ;
		token = token->next_token;
	}
	*tokens = token;
	return (pipe_tab);
}

t_container	*build_table(t_token *token, boolean is_child)
{
	t_container	*container;
	t_token		*tmp;
	int			is_group;

	is_group = false;
	tmp = token;
	container = malloc(sizeof(t_container));
	container->data = NULL;
	container->next = NULL;
	container->type = PIPE;
	container->operator = NO_OP;
	if (token->type == GroupOpen)
	{
		is_group = true;
		token = token->next_token;
		container->type = GROUP;
	}
	while (token != NULL)
	{
		if (is_group == true && is_child == true && token->type == GroupClose)
			return (container);
		container->data = new_build_pipe(&(token));
		new_print_pipe((t_pipe *)container->data);
		// if (token->type == And)
		if (token != NULL)
			printf("token value: %s\n", token->value);
		if (token != NULL)
			token = token->next_token;
	}
	return (container);
}


void	do_exec(t_ctx *ctx)
{
	(void)ctx;
	int	status;

	status = 0;
	build_table(ctx->token, false);
	// ctx->pipes = build_pipe(ctx->token);
	// status = do_pipes(ctx);
	printf("status: %d\n", status);
}

void	new_print_pipe(t_pipe *pipes)
{
	int	count;

	count = 0;
	printf(COLOR_MAGENTA_A "-------------- PIPE -------------\n");
	while (pipes != NULL)
	{
		t_args	*args;
		t_filename	*filenames;

		if (count != 0)
			printf("-----------\n");
		printf("exec_nb: %d\n", count);
		printf("IN_FD: %d\n", pipes->in_fd);
		printf("OUT_FD: %d\n", pipes->out_fd);
		// printf("ENV: %p\n", pipes->env);
		printf("NEXT_PIPE: %p\n", pipes->next);
		args = pipes->args;
		printf("Command: %s\n", pipes->cmd);
		if (pipes->nb_args > 0)
			printf("arg count: %d\n", pipes->nb_args);
		filenames = pipes->filename;
		while (args != NULL)
		{
			printf("\targ: \'%s\'\n",args->value);
			args = args->next_arg;
		}
		while (filenames != NULL)
		{
			printf("\t\tfilenames: \'%s\'\n", filenames->value);
			printf("\t\ttype: \'%d\'\n", filenames->type);
			filenames = filenames->next;
		}
		count++;
		pipes = pipes->next;
	}
	printf("--------------------------------\n" COLOR_RESET);
}

// int	status;
// status = EXIT_SUCCESS;
// while (container != NULL)
// {
// 	if (container->operator == NO_OP)
// 	{
// 		if (container->type == Group)
// 			status = do_group();
// 	}
// 	if (container->operator == AND_OP)
// 	{
// 		if (status == EXIT_SUCCESS)
// 		{
// 			if (container->type == PIPE)
// 				status = do_pipes();
// 			else if (container->type == Group)
// 				status = do_group();
// 		}
// 	}
// 	if (container->operator == OR_OP)
// 	{
// 		if (status == EXIT_FAILURE)
// 		{
// 			if (container->type == PIPE)
// 				status = do_pipes();
// 			else if (container->type == Group)
// 				status = do_group();
// 		}
// 	}
// 	container = container->next;
// }
// return (status);
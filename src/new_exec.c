/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 19:03:59 by okoca             #+#    #+#             */
/*   Updated: 2024/07/08 08:24:18 by okoca            ###   ########.fr       */
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
			token = token->next_token;
			pipe_tab->next = new_build_pipe(&(token));
			break ;
		}
		else if (token->type != Filename)
			break ;
		token = token->next_token;
	}
	*tokens = token;
	return (pipe_tab);
}

t_group	*build_group(t_token **tokens, t_operator_t operator)
{
	t_group	*new;
	t_token	*token;

	new = malloc(sizeof(t_group));
	new->pipe = NULL;
	new->operator = operator;
	new->next = NULL;
	token = *tokens;
	while (token != NULL)
	{
		new->pipe = new_build_pipe(&(token));
		if (token == NULL)
			break ;
		else if (token->type == And && token->next_token->type != GroupOpen)
		{
			token = token->next_token;
			new->next = build_group(&(token), AND_OP);
			break ;
		}
		else if (token->type == Or && token->next_token->type != GroupOpen)
		{
			token = token->next_token;
			new->next = build_group(&(token), OR_OP);
			break ;
		}
		else
			break ;
		token = token->next_token;
	}
	*tokens = token;
	return (new);
}

t_container	*build_table(t_token *token, t_operator_t operator)
{
	t_container	*container;
	// int			is_group;

	// is_group = false;
	container = malloc(sizeof(t_container));
	container->next = NULL;
	container->operator = operator;
	container->group = NULL;
	if (token->type == GroupOpen)
		token = token->next_token;
	while (token != NULL)
	{
		container->group = build_group(&(token), NO_OP);
		if (token == NULL)
			break ;
		else if (token->type == And)
		{
			if (token->next_token->type == GroupOpen)
				token = token->next_token;
			container->next = build_table(token->next_token, AND_OP);
			break ;
		}
		else if (token->type == Or)
		{
			if (token->next_token->type == GroupOpen)
				token = token->next_token;
			container->next = build_table(token->next_token, OR_OP);
			break ;
		}
		token = token->next_token;
	}
	return (container);
	// if (token->type == GroupOpen)
	// {
	// 	is_group = true;
	// 	token = token->next_token;
	// 	container->type = GROUP;
	// }
}


void	do_exec(t_ctx *ctx)
{
	(void)ctx;
	int	status;

	status = 0;
	t_container	*tab;

	tab = build_table(ctx->token, NO_OP);
	print_table(tab);
	// print_group(build_group(ctx->token, NO_OP));
	// status = do_pipes(ctx);
	printf("status: %d\n", status);
}

void	print_table(t_container *container)
{
	printf(COLOR_GREEN_A "-------------- TABLE -------------\n");
	while (container != NULL)
	{
		printf("operator: %c\n", container->operator);
		printf("next: %p\n", container->next);
		printf("group: %p\n", container->group);
		print_group(container->group);
		container = container->next;
	}
	printf("--------------------------------\n" COLOR_RESET);
}

void	print_group(t_group *group)
{
	printf(COLOR_RED_A "-------------- GROUP -------------\n");
	while (group != NULL)
	{
		printf("operator: %c\n", group->operator);
		printf("next: %p\n", group->next);
		printf("pipe: %p\n", group->pipe);
		new_print_pipe(group->pipe);
		group = group->next;
	}
	printf("--------------------------------\n" COLOR_RESET);
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
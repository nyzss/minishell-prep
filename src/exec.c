/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 19:00:56 by okoca             #+#    #+#             */
/*   Updated: 2024/07/02 11:52:46 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

t_args	*create_args(char *value)
{
	t_args	*arg;

	arg = malloc(sizeof(t_args));
	arg->value = value;
	arg->next_arg = NULL;
	return (arg);
}
t_args	*last_arg(t_args *head)
{
	while (head->next_arg != NULL)
		head = head->next_arg;
	return (head);
}

int	add_arg(t_args **head, t_args *new)
{
	if (new == NULL)
		return (1);
	if (*head == NULL)
		*head = new;
	else
		last_arg((*head))->next_arg = new;
	return (0);
}

t_cmd	*create_cmd(t_token *token)
{
	t_cmd	*cmd;
	t_token	*tok;
	char	**args;
	int		i;

	i = 0;
	args = NULL;
	tok = token->next_token;
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->value = NULL;
	if (token->value)
		cmd->value = token->value;
	cmd->extra_args = NULL;
	cmd->arg_count = 0;
	while (tok != NULL
		&& (tok->type == DoubleQuoteString
		|| tok->type == SingleQuoteString
		|| tok->type == Argument))
	{
		if (tok->type == DoubleQuoteString || tok->type == SingleQuoteString)
			tok->type = Argument;
		if (tok->type == Command)
		{
			args = ft_split(tok->value, ' ');
			while (args[i])
			{
				add_arg(&(cmd->extra_args), create_args(args[i]));
				cmd->arg_count++;
				i++;
			}
		}
		else
		{
			add_arg(&(cmd->extra_args), create_args(tok->value));
			cmd->arg_count++;
		}
		tok = tok->next_token;
	}
	// token->next_token = tok;
	return (cmd);
}

t_pipe	*build_pipe(t_token *token)
{
	t_pipe	*new;

	new = malloc(sizeof(t_pipe));
	new->cmd = NULL;
	new->in_fd = STDIN_FILENO;
	new->out_fd = STDOUT_FILENO;
	new->next = NULL;
	new->filenames = NULL;
	while (token != NULL)
	{
		if (token->type == Infile)
		{
			if (new->in_fd != STDIN_FILENO)
				close(new->in_fd);
			new->in_fd = open(token->next_token->value, O_RDONLY | __O_CLOEXEC);
			// token = token->next_token;
		}
		else if (token->type == HereDoc)
		{
			//TODO HEREDOC
			if (new->in_fd != STDIN_FILENO)
				close(new->in_fd);
			new->in_fd = handle_here_doc(new, token->next_token->value);
			// token = token->next_token;
		}
		else if (token->type == Outfile)
		{
			if (new->out_fd != STDOUT_FILENO)
				close(new->out_fd);
			new->out_fd = open(token->next_token->value, O_WRONLY | O_CREAT | O_TRUNC | __O_CLOEXEC, 0777);
			// token = token->next_token;
		}
		else if (token->type == Append)
		{
			if (new->out_fd != STDOUT_FILENO)
				close(new->out_fd);
			new->out_fd = open(token->next_token->value, O_WRONLY | O_CREAT | O_APPEND | __O_CLOEXEC, 0777);
			// token = token->next_token;
		}
		// handle_redirections(token, new);
		if (token->type == Command || token->type == DoubleQuoteString)
		{
			if (token->type == DoubleQuoteString)
				token->type = Command;
			new->cmd = create_cmd(token);
			// add_cmd(&(new->cmd), create_cmd(token));
		}
		else if (token->type == Pipe)
		{
			new->next = build_pipe(token->next_token);
			break ;
		}
		token = token->next_token;
	}
	return (new);
}

int	call_command_pipe(t_ctx *ctx, t_pipe *pipes, int last)
{
	pid_t	pid;
	int		fds[2];

	pipe(fds);
	pid = fork();
	if (pid == 0)
	{
		if (pipes->in_fd != STDIN_FILENO)
		{
			dup2(pipes->in_fd, STDIN_FILENO);
			close(pipes->in_fd);
		}
		if (last == 0)
			dup2(fds[1], STDOUT_FILENO);
		else if (pipes->out_fd != STDOUT_FILENO)
		{
			dup2(pipes->out_fd, STDOUT_FILENO);
			close(pipes->out_fd);
		}
		close(fds[0]);
		close(fds[1]);
		m_child(ctx, pipes->cmd, ctx->env);
		exit(0);
	}
	else
	{
		dup2(fds[0], STDIN_FILENO);
	}
	close(fds[0]);
	close(fds[1]);
	return (0);
}

int	do_pipes(t_ctx *ctx)
{
	t_pipe	*tmp;
	t_pipe	*tmp_pipes;
	int		last;
	t_args	*filenames;
	int		status;

	status = 0;
	last = 0;
	tmp = ctx->pipes;
	tmp_pipes = ctx->pipes;
	get_stds(ctx);
	if (tmp_pipes->next == NULL && is_builtin(tmp_pipes->cmd->value))
	{
		if (tmp->in_fd != STDIN_FILENO)
			dup2(tmp->in_fd, STDIN_FILENO);
		if (tmp->out_fd != STDOUT_FILENO)
			dup2(tmp->out_fd, STDOUT_FILENO);
		handle_built_in(ctx, tmp->cmd, &status);
		tmp_pipes = tmp_pipes->next;
	}
	while (tmp_pipes != NULL)
	{
		if (tmp_pipes->out_fd != STDOUT_FILENO || tmp_pipes->next == NULL)
			last = 1;
		else
			last = 0;
		call_command_pipe(ctx, tmp_pipes, last);
		if (tmp_pipes->in_fd != STDIN_FILENO)
			close(tmp_pipes->in_fd);
		if (tmp_pipes->out_fd != STDOUT_FILENO)
			close(tmp_pipes->out_fd);
		tmp_pipes = tmp_pipes->next;
	}
	while (tmp != NULL)
	{
		filenames = tmp->filenames;
		waitpid(-1, &status, 0);
		while (filenames != NULL)
		{
			unlink(filenames->value);
			filenames = filenames->next_arg;
		}
		tmp = tmp->next;
	}
	reset_stds(ctx);
	return (status);
}

void	print_pipe(t_pipe *pipe)
{
	int	count;

	count = 0;
	printf(COLOR_MAGENTA_A "-------------- PIPE -------------\n");
	while (pipe != NULL)
	{
		t_args	*args;
		t_args	*filenames;

		if (count != 0)
			printf("-----------\n");
		printf("exec_nb: %d\n", count);
		printf("IN_FD: %d\n", pipe->in_fd);
		printf("OUT_FD: %d\n", pipe->out_fd);
		// printf("ENV: %p\n", pipe->env);
		printf("NEXT_PIPE: %p\n", pipe->next);
		args = pipe->cmd->extra_args;
		printf("Command: %s\n", pipe->cmd->value);
		if (pipe->cmd->arg_count > 0)
			printf("arg count: %d\n", pipe->cmd->arg_count);
		filenames = pipe->filenames;
		while (filenames != NULL)
		{
			printf("\tfilenames: \'%s\'\n", filenames->value);
			filenames = filenames->next_arg;
		}
		while (args != NULL)
		{
			printf("\targ: \'%s\'\n",args->value);
			args = args->next_arg;
		}
		count++;
		pipe = pipe->next;
	}
	printf("--------------------------------\n" COLOR_RESET);
}
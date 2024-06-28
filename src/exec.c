/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 19:00:56 by okoca             #+#    #+#             */
/*   Updated: 2024/06/28 18:09:48 by okoca            ###   ########.fr       */
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

	tok = token->next_token;
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->value = token->value;
	cmd->next_cmd = NULL;
	cmd->extra_args = NULL;
	while (tok != NULL
		&& (tok->type == DoubleQuoteString
		|| tok->type == SingleQuoteString))
	{
		// printf("args found: %s\n", tok->value);
		add_arg(&(cmd->extra_args), create_args(tok->value));
		tok = tok->next_token;
	}
	token->next_token = tok;
	return (cmd);
}

t_cmd	*last_cmd(t_cmd *head)
{
	while (head->next_cmd != NULL)
		head = head->next_cmd;
	return (head);
}

int	add_cmd(t_cmd **head, t_cmd *new)
{
	if (new == NULL)
		return (1);
	if (*head == NULL)
		*head = new;
	else
		last_cmd((*head))->next_cmd = new;
	return (0);
}

void	print_exec(t_exec *exec)
{
	int	count;

	count = 0;
	printf(COLOR_BLUE "-------------- EXEC -------------\n");
	while (exec != NULL)
	{
		t_cmd	*cmds;
		t_args	*args;

		cmds = exec->cmds;
		if (count != 0)
			printf("-----------\n");
		printf("exec_nb: %d\n", count);
		printf("IN_FD: %d\n", exec->infile_fd);
		printf("OUT_FD: %d\n", exec->outfile_fd);
		printf("CMD_COUNT: %d\n", exec->cmd_count);
		printf("ENV: %p\n", exec->env);
		printf("NEXT_EXEC: %p\n", exec->next_exec);
		while (cmds != NULL)
		{
			args = cmds->extra_args;
			printf("Command: %s\n", cmds->value);
			while (args != NULL)
			{
				printf("\targ: %s\n",args->value);
				args = args->next_arg;
			}
			cmds = cmds->next_cmd;
		}
		count++;
		exec = exec->next_exec;
	}
	printf("--------------------------------\n" COLOR_RESET);
}

t_exec	*build_exec(t_token *token, char **env)
{
	t_exec	*new;

	new = malloc(sizeof(t_exec));
	new->cmds = NULL;
	new->infile_fd = STDIN_FILENO;
	new->outfile_fd = STDOUT_FILENO;
	new->next_exec = NULL;
	new->cmd_count = 0;
	new->env = env;
	while (token != NULL)
	{
		if (token->type == Infile)
		{
			new->infile_fd = open(token->next_token->value, O_RDONLY);
			token = token->next_token;
		}
		else if (token->type == Outfile)
		{
			new->outfile_fd = open(token->next_token->value, O_WRONLY | O_CREAT | O_TRUNC, 0777);
			token = token->next_token;
		}
		else if (token->type == Append)
		{
			new->outfile_fd = open(token->next_token->value, O_WRONLY | O_CREAT | O_APPEND, 0777);
			token = token->next_token;
		}
		else if (token->type == Pipe)
		{
			// recursive exec creation
			// first time i use recursivity by myself, proud of it
			if (new->outfile_fd != STDOUT_FILENO || new->infile_fd != STDIN_FILENO)
			{
				new->next_exec = build_exec(token, env);
				break ;
			}
		}
		else if (token->type == Command)
		{
			add_cmd(&(new->cmds), create_cmd(token));
			// check if next token is pipe, if true then go to the next cmd after that one.
			new->cmd_count += 1;
			while (token->next_token != NULL && token->next_token->type == Pipe)
			{
				if (token->next_token->next_token != NULL && token->next_token->next_token->type == Command)
				{
					token = token->next_token->next_token;
					add_cmd(&(new->cmds), create_cmd(token));
					new->cmd_count += 1;
				}
				else
					break ;
			}
		}
		token = token->next_token;
	}
	return (new);
}

void	do_exec(t_exec *exec)
{
	int		i;
	pid_t	pid;
	int		last;
	t_cmd	*cmds;

	i = 0;
	last = 0;
	cmds = exec->cmds;
	if (exec->cmd_count > 0)
	{
		pid = fork();
		if (pid == 0)
		{
			if (exec->infile_fd != STDIN_FILENO)
			{
				dup2(exec->infile_fd, STDIN_FILENO);
				close(exec->infile_fd);
			}
			while (cmds != NULL)
			{
				if (i == exec->cmd_count - 1)
					last = 1;
				// printf("EXEC: %s\n", cmds->value);
				call_command(cmds, exec, last);
				cmds = cmds->next_cmd;
				i++;
			}
			i = 0;
			while (i < exec->cmd_count)
			{
				wait(NULL);
				i++;
			}
			exit(EXIT_SUCCESS);
		}
		wait(NULL);
	}
}

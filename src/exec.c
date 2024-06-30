/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 19:00:56 by okoca             #+#    #+#             */
/*   Updated: 2024/06/30 17:46:03 by okoca            ###   ########.fr       */
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
	cmd->value = token->value;
	cmd->next_cmd = NULL;
	cmd->extra_args = NULL;
	cmd->arg_count = 0;
	while (tok != NULL
		&& (tok->type == DoubleQuoteString
		|| tok->type == SingleQuoteString
		|| tok->type == Command))
	{
		// printf("args found: %s\n", tok->value);
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
	printf(COLOR_BLUE_A "-------------- EXEC -------------\n");
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
			printf("\tCommand: %s\n", cmds->value);
			if (cmds->arg_count > 0)
				printf("\targ count: %d\n", cmds->arg_count);
			while (args != NULL)
			{
				printf("\t\targ: \'%s\'\n",args->value);
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
			if (new->infile_fd != STDIN_FILENO)
				close(new->infile_fd);
			new->infile_fd = open(token->next_token->value, O_RDONLY | __O_CLOEXEC);
			token = token->next_token;
		}
		else if (token->type == Outfile)
		{
			if (new->outfile_fd != STDOUT_FILENO)
				close(new->outfile_fd);
			new->outfile_fd = open(token->next_token->value, O_WRONLY | O_CREAT | O_TRUNC | __O_CLOEXEC, 0777);
			token = token->next_token;
		}
		else if (token->type == Append)
		{
			if (new->outfile_fd != STDOUT_FILENO)
				close(new->outfile_fd);
			new->outfile_fd = open(token->next_token->value, O_WRONLY | O_CREAT | O_APPEND | __O_CLOEXEC, 0777);
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
			// if (token->next_token != NULL)
			// {
			// 	if (token->next_token->type == Infile)
			// 	{
			// 		dprintf(2, "\n\n\nhello world\n\n\n");
			// 		new->next_exec = build_exec(token, env);
			// 		break ;
			// 	}
			// }
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

	pid = fork();
	if (pid == 0)
	{
		while (exec != NULL)
		{
			i = 0;
			last = 0;
			cmds = exec->cmds;
			if (exec->infile_fd != STDIN_FILENO)
			{
				dup2(exec->infile_fd, STDIN_FILENO);
				close(exec->infile_fd);
			}
			while (cmds != NULL)
			{
				if (i == exec->cmd_count - 1 && exec->next_exec == NULL)
					last = 1;
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
			if (exec->infile_fd != STDIN_FILENO)
				close(exec->infile_fd);
			if (exec->outfile_fd != STDOUT_FILENO)
				close(exec->outfile_fd);
			exec = exec->next_exec;
		}
		exit(EXIT_SUCCESS);
	}
	wait(NULL);
}


// pipe struct:

// command, args
// in_fd, out_fd

// -> next pipe struct
// typedef struct s_pipe
// {
// 	int				in_fd;
// 	int				out_fd;
// 	char			**env;
// 	t_cmd			*cmd;
// 	struct s_pipe	*next;
// }	t_pipe;

// void	handle_redirections(t_token *token, t_pipe *new)
// {

// }

t_pipe	*build_pipe(t_token *token, char **env)
{
	t_pipe	*new;

	new = malloc(sizeof(t_pipe));
	new->env = env;
	new->cmd = NULL;
	new->in_fd = STDIN_FILENO;
	new->out_fd = STDOUT_FILENO;
	new->next = NULL;
	while (token != NULL)
	{
		if (token->type == Infile)
		{
			if (new->in_fd != STDIN_FILENO)
				close(new->in_fd);
			new->in_fd = open(token->next_token->value, O_RDONLY | __O_CLOEXEC);
			token = token->next_token;
		}
		else if (token->type == HereDoc)
		{
			//TODO HEREDOC
			if (new->in_fd != STDIN_FILENO)
				close(new->in_fd);
			new->in_fd = open(token->next_token->value, O_RDONLY | __O_CLOEXEC, 0777);
			token = token->next_token;
		}
		else if (token->type == Outfile)
		{
			if (new->out_fd != STDOUT_FILENO)
				close(new->out_fd);
			new->out_fd = open(token->next_token->value, O_WRONLY | O_CREAT | O_TRUNC | __O_CLOEXEC, 0777);
			token = token->next_token;
		}
		else if (token->type == Append)
		{
			if (new->out_fd != STDOUT_FILENO)
				close(new->out_fd);
			new->out_fd = open(token->next_token->value, O_WRONLY | O_CREAT | O_APPEND | __O_CLOEXEC, 0777);
			token = token->next_token;
		}
		// handle_redirections(token, new);
		if (token->type == Command)
		{
			add_cmd(&(new->cmd), create_cmd(token));
		}
		else if (token->type == Pipe)
		{
			new->next = build_pipe(token->next_token, env);
			break ;
		}
		token = token->next_token;
	}
	return (new);
}

int	call_command_pipe(t_pipe *pipes, int last)
{
	pid_t	pid;
	int		fds[2];

	pipe(fds);
	pid = fork();
	if (pid == 0)
	{
		if (last == 0)
			dup2(fds[1], STDOUT_FILENO);
		else if (pipes->out_fd != STDOUT_FILENO)
		{
			dup2(pipes->out_fd, STDOUT_FILENO);
			close(pipes->out_fd);
		}
		// dprintf(2, "\ndeep in here\n");
		close(fds[0]);
		close(fds[1]);
		m_child(pipes->cmd, pipes->env);
	}
	else
	{
		dup2(fds[0], STDIN_FILENO);
	}
	close(fds[0]);
	close(fds[1]);
	return (0);
}

void	do_pipes(t_pipe *pipes)
{
	pid_t	pid;
	t_pipe	*tmp;
	int		last;

	last = 0;
	tmp = pipes;
	pid = fork();
	if (pid == 0)
	{
		while (pipes != NULL)
		{
			if (pipes->in_fd != STDIN_FILENO)
			{
				dup2(pipes->in_fd, STDIN_FILENO);
				close(pipes->in_fd);
			}
			if (pipes->next == NULL)
				last = 1;
			call_command_pipe(pipes, last);
			if (pipes->in_fd != STDIN_FILENO)
				close(pipes->in_fd);
			if (pipes->out_fd != STDOUT_FILENO)
				close(pipes->out_fd);
			pipes = pipes->next;
		}
		while (tmp != NULL)
		{
			wait(NULL);
			tmp = tmp->next;
		}
		exit(EXIT_SUCCESS);
	}
	wait(NULL);
}

void	print_pipe(t_pipe *pipe)
{
	int	count;

	count = 0;
	printf(COLOR_MAGENTA_A "-------------- PIPE -------------\n");
	while (pipe != NULL)
	{
		t_args	*args;

		if (count != 0)
			printf("-----------\n");
		printf("exec_nb: %d\n", count);
		printf("IN_FD: %d\n", pipe->in_fd);
		printf("OUT_FD: %d\n", pipe->out_fd);
		printf("ENV: %p\n", pipe->env);
		printf("NEXT_PIPE: %p\n", pipe->next);
		args = pipe->cmd->extra_args;
		printf("Command: %s\n", pipe->cmd->value);
		if (pipe->cmd->arg_count > 0)
			printf("arg count: %d\n", pipe->cmd->arg_count);
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
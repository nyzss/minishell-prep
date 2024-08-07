/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 11:22:48 by okoca             #+#    #+#             */
/*   Updated: 2024/07/07 21:46:35 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PREP_H
# define PREP_H

# define READ_BUFFER 1024
# define HERE_DOC_TMP 10

# define SHOULD_EXIT 2

# define true 1
# define false 0

# define PARSING_ERROR 1
// # define PARSING_ERROR 1

// # define RED_COLOR "\033[0;31m"
// # define BLUE_COLOR "\033[1;34m"
// # define RESET_COLOR "\033[0m"

# define COLOR_REDB     "\033[31m"
# define COLOR_GREENB   "\033[32m"
# define COLOR_YELLOWB  "\033[33m"
# define COLOR_BLUEB    "\033[34m"
# define COLOR_MAGENTAB "\033[35m"
# define COLOR_CYANB    "\033[36m"

# define COLOR_RED_A     "\033[1;31m"
# define COLOR_GREEN_A   "\033[1;32m"
# define COLOR_YELLOW_A  "\033[1;33m"
# define COLOR_BLUE_A    "\033[1;34m"
# define COLOR_MAGENTA_A "\033[1;35m"
# define COLOR_CYAN_A    "\033[1;36m"

// C1=$'\001\033[1;34m\002'
// C0=$'\001\033[0;0m\002'

# define ESCAPE_F "\001"
# define ESCAPE_S "\002"

# define COLOR_RESET "\033[0m"

# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <ncurses.h>
# include <termios.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>

# include "libft.h"

# ifndef DEBUG
#  define DEBUG 0
# endif

typedef enum e_token_type
{
	SingleQuoteString,
	DoubleQuoteString,
	RawString,
	Infile,
	Append,
	Outfile,
	HereDoc,
	Pipe,
	Command,
	Argument,
	Filename,
	And,
	Or,
	GroupOpen,
	GroupClose
}	t_token_type;

typedef enum e_meta_char
{
	SingleQuote = '\'',
	DoubleQuote = '\"',
	InfileChar = '<',
	OutfileChar = '>',
	PipeChar = '|',
	SpaceChar = ' ',
	OrChar = '|',
	AndChar = '&',
	ParenthesisOpen = '(',
	ParenthesisClose = ')',
}	t_meta_char;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next_token;
}	t_token;

// t_exec will have the infile - outfile redirection,
// with a pointer to t_cmd which contains all the commands,
// first cmd will have a infile directed to its stdin and
// once we get to the last cmd we redirect to outfile

typedef struct s_args
{
	char			*value;
	struct s_args	*next_arg;
}	t_args;

typedef struct s_filename
{
	char				*value;
	t_token_type		type;
	struct s_filename	*next;
}	t_filename;


typedef struct s_cmd
{
	int				arg_count;
	char			*value;
	t_args			*extra_args;
}	t_cmd;

typedef struct s_pipe
{
	int				in_fd;
	int				out_fd;
	t_filename		*filename;
	char			*cmd;
	t_args			*args;
	int				nb_args;
	struct s_pipe	*next;
}	t_pipe;

// typedef enum e_operator_t
// {
// 	NO_SIGN,
// 	AND_SIGN,
// 	OR_SIGN
// }	t_operator_t;

typedef enum e_operator_t
{
	NO_OP = 'N',
	AND_OP = 'A',
	OR_OP = 'O',
}	t_operator_t;

typedef enum e_type_t
{
	NO_TYPE,
	PIPE,
	GROUP,
}	t_type_t;

typedef struct s_operator
{
	t_operator_t		type;
	t_pipe				*cmds_t;
	struct s_operator	*next;
}	t_operator;

// cmd table is for Groups

// we will initialize the operator with the && op
// since we will also have the status initialized at 0
// then it will always execute the first command like we need it to
typedef struct s_group
{
	t_pipe			*pipe;
	t_operator_t	operator;
	struct s_group	*next;
}	t_group;

typedef struct s_container
{
	t_operator_t		operator;
	t_group				*group;
	struct s_container	*next;
}	t_container;
// union
// {
// 	struct s_container	*group;
// 	t_pipe	*pipe;
// }	*data;

/*
	(ls && (ls -l -a | cat || (cat Makefile || grep "NAME")))

	for this just recursively call create_group(), start with finding a first ( and then call it a

	from the start take it as if every command starts of as a group ()
	and if the first commands actually starts with a group () then,
	well, check for that

	if there is a group withnin a group just call the exec() function recursively.
*/

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

/*
 structure goes like this:
 we start of by considering that even 'ls | cat'
 is inside a group (parenthesis)

 any && or || signs will still contribute to
 the last status of the parenthesis since we need to
 set something for echo $?

 otherwise if we get another set of parenthesis in the line
 its just going to be another group etc..

 each Group block will have
*/

typedef	int	boolean;

typedef struct s_ctx
{
	int		def_in;
	int		def_out;
	char	*line;
	char	**env;
	t_token	*token;
	t_pipe	*pipes;
}	t_ctx;

void	print_group(t_group *group);

void	print_table(t_container *container);

t_filename	*create_filename(char *value, t_token_type type);

t_filename	*last_filename(t_filename *head);

int			add_filename(t_filename **head, t_filename *new);

t_token	*lexer(char *str);

void	do_exec(t_ctx *ctx);

int		parser(t_token *token);

void	*lex_clear_tokens(t_token *token);

char	*ps_handle_quotes(char *str);

int		p_exec(char *path, char **args, char **envp);

char	*p_strjoin(char *first, char *second, char *add);

void	p_cleanup_array(char **arr);

char	*p_get_path(char *arg);

char	*p_helper_path(char *arg, char *path, char **paths, int i);

size_t	ft_strlen(const char *str);

char	**ft_split(char const *str, char c);

void	handle_sigint(int status);

t_args	*create_args(char *value);

int		add_arg(t_args **head, t_args *new);

void	get_stds(t_ctx *ctx);

void	reset_stds(t_ctx *ctx);

char	*combine_tokens(t_token *token);

void	new_print_pipe(t_pipe *pipes);

// --------------------------------- LEXER -------------------------------

int		clear_token(t_token **token);

int		add_token(t_token **head, t_token *new);

t_token	*last_token(t_token *head);

t_token	*create_token(t_token_type type, char *value);

int		print_token(t_token *token);

// ------------------------------- PARSER (somewhat) ----------------------

// -------------------------------- HISTORY --------------------------------

void	print_history(HISTORY_STATE *state);

// --------------------------------- NEW_EXEC BUILDER ----------------------

t_pipe	*build_pipe(t_token *token);

void	print_pipe(t_pipe *pipe);

int		do_pipes(t_ctx *ctx);

int		call_command_pipe(t_ctx *ctx, t_pipe *pipes, int last);

int		m_child(t_ctx *ctx, t_cmd *cmds, char **env);

// --------------------------------- BUILTIN ------------------------------

int		handle_here_doc(t_pipe *pipes, char *filename);

int		handle_built_in(t_ctx *ctx, t_pipe *cmd, int *status);

int		handle_env_expand(t_token *token);

int		is_builtin(char *value);

int		exit_builtin(t_ctx *ctx, t_pipe *cmd);

int		expand_env(char **received);

#endif

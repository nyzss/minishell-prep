/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 11:22:48 by okoca             #+#    #+#             */
/*   Updated: 2024/06/30 17:27:16 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PREP_H
# define PREP_H

# define READ_BUFFER 1024

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

#define COLOR_RESET   "\033[0m"

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
	Filename
}	t_token_type;

typedef struct s_token
{
	char			*value;
	int				index;
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

typedef struct s_cmd
{
	int				arg_count;
	char			*value;
	t_args			*extra_args;
	struct s_cmd	*next_cmd;
}	t_cmd;

typedef struct s_exec
{
	int				infile_fd;
	int				outfile_fd;
	int				cmd_count;
	char			**env;
	t_cmd			*cmds;
	struct s_exec	*next_exec;
}	t_exec;

typedef struct s_pipe
{
	int				in_fd;
	int				out_fd;
	char			**env;
	t_cmd			*cmd;
	struct s_pipe	*next;
}	t_pipe;

int		p_exec(char *path, char **args, char **envp);

char	*p_strjoin(char *first, char *second, char *add);

void	p_cleanup_array(char **arr);

char	*p_get_path(char *arg);

char	*p_helper_path(char *arg, char *path, char **paths, int i);

size_t	ft_strlen(const char *str);

char	**ft_split(char const *str, char c);

// --------------------------------- LEXER -------------------------------

int		clear_token(t_token **token);

t_token	*tokenize_line(char *buf);

int		add_token(t_token **head, t_token *new);

t_token	*last_token(t_token *head);

t_token	*create_token(t_token_type type, char *value, int index);

int		print_token(t_token *token);

// ------------------------------- PARSER (somewhat) ----------------------

int		token_checker(t_token *token);

char	**split_once(char *value);

// -------------------------------- EXEC BUILDER --------------------------

t_exec	*build_exec(t_token *token, char **env);

void	print_exec(t_exec *exec);

int		call_command(t_cmd *cmds, t_exec *exec, int last);

void	do_exec(t_exec *exec);

// -------------------------------- BUILTIN -------------------------------

int		handle_env_expand(t_token *token);


// -------------------------------- HISTORY --------------------------------

void	print_history(HISTORY_STATE *state);

// --------------------------------- NEW_EXEC BUILDER ----------------------

t_pipe	*build_pipe(t_token *token, char **env);

void	print_pipe(t_pipe *pipe);

void	do_pipes(t_pipe *pipes);

int		m_child(t_cmd *cmds, char **env);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 11:22:48 by okoca             #+#    #+#             */
/*   Updated: 2024/06/25 07:54:38 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PREP_H
# define PREP_H

# define READ_BUFFER 1024

# define RED_COLOR "\033[0;31m"
# define BLUE_COLOR "\033[1;34m"
# define RESET_COLOR "\033[0m"

# include <readline/readline.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>

# include "libft.h"

typedef enum e_token_type
{
	DoubleQuote,
	String,
	SingleQuote,
	Infile,
	Outfile,
}	t_token_type;

typedef struct s_token
{
	char			*value;
	int				index;
	t_token_type	type;
	struct s_token	*next_token;
}	t_token;

int		p_exec(char *path, char **args, char **envp);

char	*p_get_env(char **env);

char	*p_strjoin(char *first, char *second, char *add);

void	p_cleanup_array(char **arr);

char	*p_get_path(char *arg, char **env);

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

#endif

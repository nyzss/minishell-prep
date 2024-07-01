/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 19:59:24 by okoca             #+#    #+#             */
/*   Updated: 2024/07/01 21:43:02 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

int	handle_env_expand(t_token *token)
{
	char	*found;
	char	*str;
	char	*tmp;
	int		env_len;

	while (token != NULL)
	{
		env_len = 0;
		tmp = NULL;
		if (token->type == DoubleQuoteString
			|| token->type == Argument)
		{
			found = ft_strchr(token->value, '$');
			while (found != NULL)
			{
				found = ft_strchr(token->value, '$');
				if (found != NULL)
				{
					while (found[env_len] && found[env_len] != ' ')
						env_len++;
					str = ft_strndup(found, env_len);
					str = getenv(++str);
					if (found - token->value == 0)
						tmp = ft_strdup("");
					else
						tmp = ft_strndup(token->value, found - token->value);
					if (str != NULL)
						tmp = ft_strjoin(tmp, str);
					tmp = ft_strjoin(tmp, token->value + env_len + (found - token->value));
					token->value = tmp;
				}
			}
		}
		token = token->next_token;
	}
	return (0);
}

unsigned int	pseudo_rand_num(int i)
{
	static unsigned int	seed;

    seed = (1103515245 * seed + 12345) & 0x7fffffff;
	return (i * seed);
}

char	*random_name(char *filename)
{
	char	*new;
	int		i;

	i = 0;
	new = malloc(sizeof(char) * (HERE_DOC_TMP + 1));
	new[0] = '.';
	i++;
	while (i < HERE_DOC_TMP)
	{
		new[i] = 'A' + (pseudo_rand_num(i + filename[0]) % 26);
		i++;
	}
	new[i] = '\0';
	return (new);
}

int	handle_here_doc(t_pipe *pipes, char *filename)
{
	char	*line;
	int		fd;
	char	*tmp;

	line = NULL;
	printf("\n");
	tmp = random_name(filename);
	printf("random name: %s\n", tmp);
	fd = open(tmp, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
		printf(COLOR_RED_A "fd not valid!\n\n");
	while (1)
	{
		dprintf(2, "> ");
		line = get_next_line(STDIN_FILENO);
		// line = readline("> ");
		if (ft_strcmp(line, filename) == '\n')
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
	close(fd);
	fd = open(tmp, O_RDONLY | __O_CLOEXEC);
	if (fd < 0)
		printf(COLOR_RED_A "fd not valid -- second one!!\n\n");
	add_arg(&(pipes->filenames), create_args(tmp));
	return (fd);
}

int	exit_builtin(t_cmd *cmd)
{
	t_args			*args;
	unsigned char	code;

	args = cmd->extra_args;
	if (cmd->arg_count > 1)
	{
		ft_fprintf(2, "exit\nexit: too many arguments\n");
		return (1);
	}
	code = ft_atoi(args->value);
	printf("return: %d\n", code);
	return (code);
}

int	handle_built_in(t_cmd *cmd)
{
	printf("received: %s\n", cmd->value);
	if (ft_strcmp(cmd->value, "exit") == 0)
	{
		return(exit_builtin(cmd));
	}
	return (0);
}

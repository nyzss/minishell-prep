/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 19:59:24 by okoca             #+#    #+#             */
/*   Updated: 2024/07/04 10:42:11 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

int	expand_env(char **received)
{
	char	*found;
	char	*str;
	char	*tmp;
	int		env_len;
	char	*new;

	env_len = 0;
	tmp = NULL;
	new = *received;
	while (1)
	{
		found = ft_strchr(new, '$');
		if (found == NULL)
			break ;
		while (found[env_len] && found[env_len] != ' ')
			env_len++;
		str = ft_strndup(found, env_len);
		str = getenv(++str);
		if (found - new == 0)
			tmp = ft_strdup("");
		else
			tmp = ft_strndup(new, found - new);
		if (str != NULL)
			tmp = ft_strjoin(tmp, str);
		tmp = ft_strjoin(tmp, new + env_len + (found - new));
		new = tmp;
	}
	*received = new;
	return (0);
}

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

void	free_all(t_ctx *ctx)
{
	if (ctx->line)
		free(ctx->line);
	if (ctx->pipes)
		free(ctx->pipes);
	if (ctx->token)
		free(ctx->token);
}

int	exit_builtin(t_ctx *ctx, t_cmd *cmd)
{
	t_args			*args;
	unsigned char	code;

	code = 0;
	args = cmd->extra_args;
	if (cmd->arg_count > 1)
	{
		ft_fprintf(2, "exit\nexit: too many arguments\n");
		return (1);
	}
	if (args)
		code = ft_atoi(args->value);
	free_all(ctx);
	exit(code);
}

int	cd_builtin(t_cmd *cmd)
{
	t_args	*args;

	args = cmd->extra_args;
	if (cmd->arg_count > 1)
	{
		ft_fprintf(2, "cd: \ntoo many arguments\n");
		return (1);
	}
	if (args)
		chdir(args->value);
	return (0);
}

int	pwd_builtin()
{
	char	*str;
	int		size;

	size = 128;
	str = ft_calloc(sizeof(char), size);
	if (!str)
		return (-1);
	while (getcwd(str, size) == NULL)
	{
		size *= 2;
		free(str);
		str = ft_calloc(sizeof(char), size);
	}
	printf("%s\n", str);
	free(str);
	return (0);
}

int	env_builtin(t_ctx *ctx, t_cmd *cmd)
{
	int		i;

	i = 0;
	if (cmd->arg_count > 0)
	{
		ft_fprintf(2, "cd: \ntoo many arguments\n");
		return (2);
	}
	while (ctx->env && ctx->env[i])
	{
		printf("%s\n", ctx->env[i]);
		i++;
	}
	return (0);
}

int	unset_builtin(t_ctx *ctx, t_cmd *cmd)
{
	int		i;
	t_args	*args;
	// char	**new_env;

	i = 0;
	while (ctx->env && ctx->env[i])
	{
		args = cmd->extra_args;
		while (args != NULL)
		{
			if (ft_strncmp(ctx->env[i], args->value, ft_strlen(args->value)) == 0
				&& ctx->env[i][ft_strlen(args->value)] == '=')
			{
				printf("found it: %s\n", ctx->env[i]);
				break ;
			}
			args = args->next_arg;
		}
		i++;
	}
	return (0);
}

int	is_builtin(char *value)
{
	if (ft_strcmp(value, "exit") == 0)
		return (1);
	else if (ft_strcmp(value, "cd") == 0)
		return (1);
	else if (ft_strcmp(value, "pwd") == 0)
		return (1);
	else if (ft_strcmp(value, "env") == 0)
		return (1);
	else if (ft_strcmp(value, "unset") == 0)
		return (1);
	else if (ft_strcmp(value, "export") == 0)
		return (1);
	return (0);
}

int	handle_built_in(t_ctx *ctx, t_cmd *cmd, int *status)
{
	int	found;

	found = 0;
	if (ft_strcmp(cmd->value, "exit") == 0)
	{
		*status = exit_builtin(ctx, cmd);
		found = 1;
	}
	else if (ft_strcmp(cmd->value, "cd") == 0)
	{
		*status = cd_builtin(cmd);
		found = 1;
	}
	else if (ft_strcmp(cmd->value, "pwd") == 0)
	{
		*status = pwd_builtin();
		found = 1;
	}
	else if (ft_strcmp(cmd->value, "env") == 0)
	{
		*status = env_builtin(ctx, cmd);
		found = 1;
	}
	else if (ft_strcmp(cmd->value, "unset") == 0)
	{
		*status = unset_builtin(ctx, cmd);
		found = 1;
	}
	return (found);
}

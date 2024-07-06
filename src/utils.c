/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:53:22 by okoca             #+#    #+#             */
/*   Updated: 2024/07/06 19:09:27 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

int	p_precheck_path(char *av)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	if (av[0] == '\0')
		return (1);
	while (av[i])
	{
		if (av[i] != ' ')
			n++;
		i++;
	}
	if (n == 0)
		return (1);
	return (1);
}

char	*p_strjoin(char *first, char *second, char *add)
{
	int		first_len;
	int		second_len;
	int		add_len;
	char	*new;

	add_len = ft_strlen(add);
	first_len = ft_strlen(first);
	second_len = ft_strlen(second);
	new = malloc(sizeof(char) * (first_len + add_len + second_len + 1));
	if (!new)
		return (NULL);
	ft_strcat(new, first);
	ft_strcat(new, add);
	ft_strcat(new, second);
	new[first_len + add_len + second_len + 1] = '\0';
	return (new);
}

char	*p_helper_path(char *arg, char *path, char **paths, int i)
{
	char	*tmp;

	tmp = NULL;
	if (path)
	{
		free(path);
		path = NULL;
	}
	tmp = ft_strjoin(paths[i], "/");
	if (!tmp)
		return (NULL);
	path = ft_strjoin(tmp, arg);
	free(tmp);
	if (!path)
		return (NULL);
	return (path);
}

char	*p_get_path(char *arg)
{
	char	*path;
	char	**paths;
	int		i;

	i = 0;
	if (getenv("PATH") == NULL)
		return (ft_strdup(arg));
	paths = ft_split(getenv("PATH"), ':');
	if (!paths)
		return (NULL);
	path = ft_strdup(arg);
	if (!path)
	{
		p_cleanup_array(paths);
		return (NULL);
	}
	while (access(path, F_OK | X_OK) != 0 && paths[i])
	{
		path = p_helper_path(arg, path, paths, i);
		i++;
	}
	p_cleanup_array(paths);
	return (path);
}

void	p_cleanup_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	p_exec(char *path, char **args, char **envp)
{
	if (execve(path, args, envp) < 0)
	{
		fprintf(stderr, "monk: ERROR at exec!\n");
		free(path);
		p_cleanup_array(args);
		return (1);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:06:44 by okoca             #+#    #+#             */
/*   Updated: 2024/07/05 14:23:22 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

char	*ps_handle_quotes(char *str)
{
	t_token	*tmp_token;
	int		i;
	int		j;
	int		len;
	char	*tmp;
	char	*new;

	i = 0;
	tmp_token = NULL;
	tmp = NULL;
	new = NULL;
	len = ft_strlen(str);
	while (i < len)
	{
		j = 1;
		if (str[i] == '\'')
		{
			while (i + j < len && str[i + j] != '\'')
				j++;
			tmp = ft_strndup(&(str[i + 1]), j - 1);
			add_token(&(tmp_token), create_token(SingleQuoteString, tmp));
			i += j;
		}
		else if (str[i] == '\"')
		{
			while (i + j < len && str[i + j] != '\"')
				j++;
			tmp = ft_strndup(&(str[i + 1]), j - 1);
			add_token(&(tmp_token), create_token(DoubleQuoteString, tmp));
			i += j;
		}
		else
		{
			while (i + j < len && str[i + j] != '\"' && str[i + j] != '\'')
				j++;
			tmp = ft_strndup(&(str[i]), j);
			add_token(&(tmp_token), create_token(RawString, tmp));
			i += j - 1;
		}
		i++;
	}
	handle_env_expand(tmp_token);
	new = combine_tokens(tmp_token);
	return (new);
}

char	*combine_tokens(t_token *token)
{
	t_token	*tmp;
	int		len;
	char	*new;

	len = 0;
	tmp = token;
	new = NULL;
	while (tmp != NULL)
	{
		len += ft_strlen(tmp->value);
		tmp = tmp->next_token;
	}
	new = ft_calloc(sizeof(char), (len + 1));
	while (token != NULL)
	{
		if (token->value != NULL)
			ft_strcat(new, token->value);
		token = token->next_token;
	}
	return (new);
}

int	handle_env_expand(t_token *token)
{
	char	*found;
	char	*str;
	char	*tmp;
	int		env_len;

	while (token != NULL)
	{
		env_len = 1;
		tmp = NULL;
		if (token->type == DoubleQuoteString
			|| token->type == RawString)
		{
			while (1)
			{
				found = ft_strchr(token->value, '$');
				if (found == NULL)
					break ;
				while (found[env_len] && ft_isalnum(found[env_len]))
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
		token = token->next_token;
	}
	return (0);
}

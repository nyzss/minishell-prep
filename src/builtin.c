/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 19:59:24 by okoca             #+#    #+#             */
/*   Updated: 2024/06/29 22:08:54 by okoca            ###   ########.fr       */
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
		if (token->type == DoubleQuoteString
			|| token->type == Command)
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
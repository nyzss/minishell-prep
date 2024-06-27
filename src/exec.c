/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 19:00:56 by okoca             #+#    #+#             */
/*   Updated: 2024/06/27 08:59:03 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

t_exec	*build_exec(t_token *token)
{
	t_exec	new;

	new.infile_fd = STDIN_FILENO;
	new.outfile_fd = STDOUT_FILENO;
	while (token != NULL)
	{
		// if (token->type == Infile)
			// new.infile_fd = open();
	}
}
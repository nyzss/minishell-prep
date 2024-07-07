/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 08:46:41 by okoca             #+#    #+#             */
/*   Updated: 2024/07/07 09:26:00 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

t_group	*create_group()
{
	t_group	*new;

	new = malloc(sizeof(t_group));
	new->next = NULL;
	return (new);
	// new.
	// return (new);
}

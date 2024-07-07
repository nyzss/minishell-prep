/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 08:46:41 by okoca             #+#    #+#             */
/*   Updated: 2024/07/07 21:37:09 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

t_filename	*create_filename(char *value, t_token_type type)
{
	t_filename	*filename;

	filename = malloc(sizeof(t_filename));
	filename->value = value;
	filename->type = type;
	filename->next = NULL;
	return (filename);
}
t_filename	*last_filename(t_filename *head)
{
	while (head->next != NULL)
		head = head->next;
	return (head);
}

int	add_filename(t_filename **head, t_filename *new)
{
	if (new == NULL)
		return (1);
	if (*head == NULL)
		*head = new;
	else
		last_filename((*head))->next = new;
	return (0);
}


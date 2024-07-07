/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 08:46:41 by okoca             #+#    #+#             */
/*   Updated: 2024/07/07 15:06:48 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

t_container	*create_container(t_token *token)
{
	t_container	*new;

	(void)token;
	new = malloc(sizeof(t_container));
	new->next = NULL;
	new->operator = NO_OP;
	new->type = NO_TYPE;
	new->data = NULL;
	return (new);
}

t_container	*last_container(t_container *head)
{
	while (head->next != NULL)
		head = head->next;
	return (head);
}

int	add_container(t_container **head, t_container *new)
{
	if (new == NULL)
		return (1);
	if (*head == NULL)
		*head = new;
	else
		last_container((*head))->next = new;
	return (0);
}

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


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 22:27:18 by okoca             #+#    #+#             */
/*   Updated: 2024/06/30 12:18:21 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prep.h"

void	print_history(HISTORY_STATE *state)
{
	int	i;

	i = 0;
	printf(COLOR_GREEN_A "------------ HISTORY -----------\n");
	printf("size: %d\n", state->size);
	printf("length: %d\n", state->length);
	printf("offset: %d\n", state->offset);
	while (state->entries[i])
	{
		printf("\t[%d] %s\n", i, state->entries[i]->line);
		// printf("\ttimestamp: %s\n", state->entries[i]->timestamp);
		i++;
	}
	printf("--------------------------------\n" COLOR_RESET);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 22:27:18 by okoca             #+#    #+#             */
/*   Updated: 2024/06/30 12:54:40 by okoca            ###   ########.fr       */
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

// void	handle_arrow_keys()
// {
// 	struct termios	term;
// 	struct termios	raw;

// 	tcgetattr(STDIN_FILENO, &term);
// 	raw.c_lflag &= ~(ECHO | ICANON);
// 	raw.c_cc[VMIN] = 1;
// 	raw.c_cc[VTIME] = 0;
// 	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

// 	while (1)
// 	{
// 		char seq[3];
// 		if (read(STDIN_FILENO, &seq[0], 1) != 1) break;
// 		if (seq[0] == 27) {
// 			if (read(STDIN_FILENO, &seq[1], 1) != 1) break;
// 			if (read(STDIN_FILENO, &seq[2], 1) != 1) break;

// 			if (seq[1] == '[') {
// 				if (seq[2] == 'A')
// 				{
// 					printf("\nUp arrow key pressed\n");
// 				}
// 				else if (seq[2] == 'B')
// 				{
// 					printf("\nDown arrow key pressed\n");
// 				}
// 			}
// 		}
// 		else
// 		{
// 			ungetc(seq[0], stdin);
// 			break;
// 		}
// 	}

// 	tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
// 	printf("hello world!\n");
// }

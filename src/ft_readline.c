#include "lexer.h"

int	ft_prompt(char *prompt)
{
	write(1, BLUE_COLOR, strlen(RED_COLOR));
	write(1, prompt, strlen(prompt));
	write(1, RESET_COLOR, strlen(RESET_COLOR));
	return (0);
}

char	*ft_readline(char *prompt)
{
	char	*buf;
	int		total_size;
	int		b_read;
	int		index;

	b_read = 1;
	index = 0;
	total_size = READ_BUFFER;
	buf = malloc(sizeof(char) * (total_size + 1));
	ft_prompt(prompt);
	while (1)
	{
		if (index == total_size)
		{
			total_size *= 2;
			buf = realloc(buf, sizeof(char) * total_size);
		}
		b_read = read(1, &buf[index], 1);
		if (buf[index] == '\n' || buf[index] == '\0')
			break ;
		index++;
	}
	buf[index] = '\0';
	return (buf);
}


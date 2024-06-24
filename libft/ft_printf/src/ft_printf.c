/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:17:17 by okoca             #+#    #+#             */
/*   Updated: 2024/06/12 08:12:31 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	handle_flags_fd(int fd, va_list args, char c)
{
	int	printed_char;

	printed_char = 0;
	if (c == 'c')
		printed_char += c_flag(fd, args);
	else if (c == 's')
		printed_char += s_flag(fd, args);
	else if (c == 'd')
		printed_char += d_flag(fd, args);
	else if (c == 'p')
		printed_char += p_flag(fd, args);
	else if (c == 'i')
		printed_char += d_flag(fd, args);
	else if (c == 'u')
		printed_char += u_flag(fd, args);
	else if (c == 'x')
		printed_char += x_flag(fd, args);
	else if (c == 'X')
		printed_char += big_x_flag(fd, args);
	else if (c == '%')
		printed_char += percent_flag(fd);
	return (printed_char);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		i;
	int		printed_char_nb;

	i = 0;
	printed_char_nb = 0;
	if (!str)
		return (-1);
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			printed_char_nb += handle_flags_fd(1, args, str[i + 1]);
			i++;
		}
		else
		{
			printed_char_nb++;
			ft_putchar_fd(str[i], 1);
		}
		i++;
	}
	va_end(args);
	return (printed_char_nb);
}

int	ft_fprintf(int fd, const char *str, ...)
{
	va_list	args;
	int		i;
	int		printed_char_nb;

	i = 0;
	printed_char_nb = 0;
	if (!str)
		return (-1);
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			printed_char_nb += handle_flags_fd(fd, args, str[i + 1]);
			i++;
		}
		else
		{
			printed_char_nb++;
			ft_putchar_fd(str[i], fd);
		}
		i++;
	}
	va_end(args);
	return (printed_char_nb);
}

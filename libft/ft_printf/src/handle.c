/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 16:37:31 by okoca             #+#    #+#             */
/*   Updated: 2024/06/12 08:09:18 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	x_flag(int fd, va_list args)
{
	int	d;
	int	count;

	count = 0;
	d = va_arg(args, int);
	putn(fd, (unsigned int)d, "0123456789abcdef", &count);
	return (count);
}

int	big_x_flag(int fd, va_list args)
{
	int	d;
	int	count;

	count = 0;
	d = va_arg(args, int);
	putn(fd, (unsigned int)d, "0123456789ABCDEF", &count);
	return (count);
}

int	percent_flag(int fd)
{
	ft_putchar_fd('%', fd);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:11:00 by okoca             #+#    #+#             */
/*   Updated: 2024/06/12 08:13:21 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	putn(int fd, unsigned long num, char *base, int *count)
{
	unsigned int	base_size;

	base_size = ft_strlen(base);
	if (num > (base_size - 1))
		putn(fd, num / base_size, base, count);
	ft_putchar_fd(base[num % base_size], fd);
	*count = *count + 1;
}

int	ft_putnbr_base(int fd, int nbr, char *base)
{
	long	num;
	int		count;

	count = 0;
	num = nbr;
	if (num < 0)
	{
		num *= -1;
		count++;
		ft_putchar_fd('-', fd);
	}
	putn(fd, num, base, &count);
	return (count);
}

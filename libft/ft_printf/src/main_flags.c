/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_flags.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 12:20:40 by okoca             #+#    #+#             */
/*   Updated: 2024/06/12 08:06:48 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	c_flag(int fd, va_list args)
{
	char	c;

	c = va_arg(args, int);
	ft_putchar_fd(c, fd);
	return (1);
}

int	s_flag(int fd, va_list args)
{
	char	*str;

	str = va_arg(args, char *);
	if (!str)
	{
		ft_putstr_fd("(null)", fd);
		return (6);
	}
	ft_putstr_fd(str, fd);
	return (ft_strlen(str));
}

int	p_flag(int fd, va_list args)
{
	void	*ptr;
	int		count;

	count = 2;
	ptr = va_arg(args, void *);
	if (!ptr)
	{
		ft_putstr_fd("(nil)", fd);
		return (5);
	}
	ft_putstr_fd("0x", fd);
	putn(fd, (unsigned long)(ptr), "0123456789abcdef", &count);
	return (count);
}

int	d_flag(int fd, va_list args)
{
	int	d;

	d = va_arg(args, int);
	return (ft_putnbr_base(fd, d, "0123456789"));
}

int	u_flag(int fd, va_list args)
{
	unsigned int	d;
	int				count;

	count = 0;
	d = va_arg(args, unsigned int);
	putn(fd, d, "0123456789", &count);
	return (count);
}

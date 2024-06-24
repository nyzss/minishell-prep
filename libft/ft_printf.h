/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 14:27:41 by okoca             #+#    #+#             */
/*   Updated: 2024/06/12 08:09:47 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include "libft.h"

//attribute pour avoir les memes erreur et le format du vrai printf
int		ft_printf(const char *str, ...)
		__attribute__ ((format (printf, 1, 2)));

int		ft_fprintf(int fd, const char *str, ...)
		__attribute__ ((format (printf, 2, 3)));

int		handle_flags_fd(int fd, va_list args, char c);

int		ft_putnbr_base(int fd, int nbr, char *base);

void	putn(int fd, unsigned long num, char *base, int *count);

int		c_flag(int fd, va_list args);

int		s_flag(int fd, va_list args);

int		p_flag(int fd, va_list args);

int		d_flag(int fd, va_list args);

int		u_flag(int fd, va_list args);

int		x_flag(int fd, va_list args);

int		big_x_flag(int fd, va_list args);

int		percent_flag(int fd);

#endif

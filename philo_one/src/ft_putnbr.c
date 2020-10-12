/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 17:51:16 by deddara           #+#    #+#             */
/*   Updated: 2020/10/12 17:51:17 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void		ft_recursion(int n, int fd)
{
	char c;

	if (n >= 10)
		ft_recursion(n / 10, fd);
	c = n % 10 + '0';
	write(fd, &c, 1);
}

void			ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
		write(fd, "-2147483648", 11);
	else if (n == 0)
		write(fd, "0", 1);
	else if (n < 0)
	{
		write(fd, "-", 1);
		n = n * (-1);
	}
	if (n > 0)
		ft_recursion(n, fd);
}

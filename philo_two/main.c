/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 17:51:23 by deddara           #+#    #+#             */
/*   Updated: 2020/10/12 17:51:24 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int				check_validation(int argc, char **argv)
{
	int i;

	i = 1;
	if (argc < 5 || argc > 6)
	{
		ft_putstr_fd("invalid number of arguments\n", 2);
		return (1);
	}
	while (argv[i])
	{
		if (check_for_num(argv[i]))
		{
			ft_putstr_fd("wrong number: ", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd("\n", 2);
			return (1);
		}
		i++;
	}
	return (0);
}

int				main(int argc, char **argv)
{
	t_table		table;

	if (check_validation(argc, argv))
		return (1);
	if (init_and_threads(&table, argv))
		return (1);
	return (0);
}

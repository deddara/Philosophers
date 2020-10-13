/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 12:08:07 by deddara           #+#    #+#             */
/*   Updated: 2020/10/13 12:08:08 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void		kill_processes(t_table *table)
{
	int i;

	i = 0;
	while (i < table->phl_num)
	{
		kill(table->pid[i], SIGKILL);
		i++;
	}
}

void		*check_eat(void *val)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo*)val;
	i = 0;
	while (i < philo->table->phl_num)
	{
		sem_wait(philo[i].finish_eat);
		i++;
	}
	sem_post(philo->table->finish);
	return (NULL);
}

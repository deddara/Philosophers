/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_threads.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 18:19:38 by deddara           #+#    #+#             */
/*   Updated: 2020/10/12 18:19:39 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void		init_philo(int i, t_philo *philo, t_table *table)
{
	philo->id = i;
	philo->table = table;
	philo->eat_num = table->eat_num;
}

static void		start_threads(t_table *table)
{
	int			i;
	t_philo		philo[table->phl_num];
	pthread_t	thread[table->phl_num];

	i = 0;
	while (i < table->phl_num)
	{
		init_philo(i, &philo[i], table);
		pthread_create(&thread[i], NULL, simulation, &philo[i]);
		i++;
	}
	i = 0;
	while (i < table->phl_num)
	{
		pthread_join(thread[i], NULL);
		i++;
	}
}

static void		init_semaphores_and_start_threads(t_table *table)
{
	sem_t					*death_sem;
	sem_t					*waiter;
	sem_t					*forks;

	sem_unlink("forks");
	sem_unlink("waiter");
	sem_unlink("death_sem");
	forks = sem_open("forks", O_CREAT, 0660, table->phl_num);
	death_sem = sem_open("death_sem", O_CREAT, 0660, 1);
	waiter = sem_open("waiter", O_CREAT, 0660, 1);
	table->forks = forks;
	table->steward = waiter;
	table->death_sem = death_sem;
	start_threads(table);
	sem_close(forks);
	sem_close(waiter);
	sem_close(death_sem);
}

int				init_and_threads(t_table *table, char **argv)
{
	if (!(table->phl_num = ft_atoi(argv[1])))
	{
		ft_putstr_fd("there are no philosophers\n", 2);
		return (1);
	}
	table->die_time = ft_atoi(argv[2]);
	table->eat_time = ft_atoi(argv[3]);
	table->sleep_time = ft_atoi(argv[4]);
	if (!(argv[5]))
		table->eat_num = -1;
	else
		table->eat_num = ft_atoi(argv[5]);
	table->smb_died = 0;
	init_semaphores_and_start_threads(table);
	return (0);
}

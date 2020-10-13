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
		if (pthread_create(&thread[i], NULL, simulation, &philo[i]) != 0)
			return ;
		i++;
	}
	i = 0;
	while (i < table->phl_num)
	{
		if (pthread_join(thread[i], NULL) != 0)
			return ;
		i++;
	}
}

static int		open_semaphores(t_table *table)
{
	sem_t					*death_sem;
	sem_t					*waiter;
	sem_t					*forks;

	if ((forks = sem_open("forks", O_CREAT, 0660, table->phl_num)) < 0)
		return (1);
	if ((death_sem = sem_open("death_sem", O_CREAT, 0660, 1)) < 0)
		return (1);
	if ((waiter = sem_open("waiter", O_CREAT, 0660, 1)) < 0)
		return (1);
	if ((table->output = sem_open("output", O_CREAT, 0660, 1)) < 0)
		return (1);
	if ((table->time_sem = sem_open("time_sem", O_CREAT, 0660, 1)) < 0)
		return (1);
	table->forks = forks;
	table->steward = waiter;
	table->death_sem = death_sem;
	start_threads(table);
	sem_close(forks);
	sem_close(table->output);
	sem_close(waiter);
	sem_close(table->time_sem);
	sem_close(death_sem);
	return (0);
}

static int		init_semaphores_and_start_threads(t_table *table)
{
	sem_unlink("forks");
	sem_unlink("waiter");
	sem_unlink("death_sem");
	sem_unlink("output");
	sem_unlink("time_sem");
	if (open_semaphores(table))
	{
		write(2, "system error\n", 13);
		return (1);
	}
	return (0);
}

int				init_and_threads(t_table *table, char **argv)
{
	if (!(table->phl_num = ft_atoi(argv[1])))
	{
		ft_putstr_fd("there are no philosophers\n", 2);
		return (1);
	}
	if (table->phl_num > 200)
	{
		ft_putstr_fd("too much philosophers\n", 2);
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
	if (init_semaphores_and_start_threads(table))
		return (1);
	return (0);
}

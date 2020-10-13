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

static void		philo_process(t_philo *philo, t_table *table, int i)
{
	pid_t pid;

	if ((pid = fork()) < 0)
	{
		write(2, "fork error\n", 11);
		return ;
	}
	if (pid == 0)
	{
		simulation(philo);
		while (1)
			usleep(1000);
	}
	else
	{
		table->pid[i] = pid;
	}
}

static void		start_processes(t_table *table)
{
	int			i;
	pid_t		pid[table->phl_num];
	t_philo		philo[table->phl_num];
	pthread_t	eat_thread;

	i = 0;
	table->pid = pid;
	while (i < table->phl_num)
	{
		init_philo(i, &philo[i], table);
		philo_process(&philo[i], table, i);
		i++;
	}
	if (pthread_create(&eat_thread, NULL, check_eat, philo) != 0)
	{
		ft_putstr_fd("thread error", 2);
		return ;
	}
	sem_wait(table->finish);
	kill_processes(table);
}

static int		open_semaphores(t_table *table)
{
	sem_t					*death_sem;
	sem_t					*forks;

	if ((table->finish = sem_open("finish", O_CREAT, 0660, 0)) < 0)
		return (1);
	if ((forks = sem_open("forks", O_CREAT, 0660, table->phl_num)) < 0)
		return (1);
	if ((death_sem = sem_open("death_sem", O_CREAT, 0660, 1)) < 0)
		return (1);
	if ((table->steward = sem_open("waiter", O_CREAT, 0660, 1)) < 0)
		return (1);
	if ((table->output_sem = sem_open("output_sem", O_CREAT, 0660, 1)) < 0)
		return (1);
	if ((table->time_sem = sem_open("time_sem", O_CREAT, 0660, 1)) < 0)
		return (1);
	table->forks = forks;
	table->death_sem = death_sem;
	start_processes(table);
	sem_close(forks);
	sem_close(table->output_sem);
	sem_close(table->steward);
	sem_close(table->finish);
	sem_close(table->time_sem);
	sem_close(death_sem);
	return (0);
}

static int		init_semaphores_and_start_processes(t_table *table)
{
	sem_unlink("output_sem");
	sem_unlink("forks");
	sem_unlink("finish");
	sem_unlink("waiter");
	sem_unlink("death_sem");
	sem_unlink("time_sem");
	if (open_semaphores(table))
	{
		write(2, "system error\n", 13);
		return (1);
	}
	return (0);
}

int				init_and_processes(t_table *table, char **argv)
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
	if (init_semaphores_and_start_processes(table))
		return (1);
	return (0);
}

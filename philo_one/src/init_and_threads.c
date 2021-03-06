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

static void		start_threads(t_table *table, pthread_mutex_t *forks)
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
	i = 0;
	while (i < table->phl_num)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

static void		init_mutex_and_start_threads(t_table *table)
{
	int						i;
	pthread_mutex_t			forks[table->phl_num];
	static pthread_mutex_t	death_mutex = PTHREAD_MUTEX_INITIALIZER;
	static pthread_mutex_t	output_mutex = PTHREAD_MUTEX_INITIALIZER;
	static pthread_mutex_t	time_mutex = PTHREAD_MUTEX_INITIALIZER;

	i = 0;
	while (i < table->phl_num)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	table->forks = forks;
	table->output_mutex = output_mutex;
	table->death_mutex = death_mutex;
	table->time_mutex = time_mutex;
	start_threads(table, forks);
	pthread_mutex_destroy(&death_mutex);
	pthread_mutex_destroy(&time_mutex);
	pthread_mutex_destroy(&output_mutex);
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
	init_mutex_and_start_threads(table);
	return (0);
}

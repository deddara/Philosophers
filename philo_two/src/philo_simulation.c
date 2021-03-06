/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 18:19:41 by deddara           #+#    #+#             */
/*   Updated: 2020/10/12 18:19:42 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int		forks_handler(t_philo *philo)
{
	sem_wait(philo->table->steward);
	sem_wait(philo->table->forks);
	if (philo->table->smb_died)
	{
		sem_post(philo->table->steward);
		sem_post(philo->table->forks);
		return (1);
	}
	msg(philo, "has taken a fork\n");
	if (philo->table->phl_num == 1)
	{
		while (!philo->table->smb_died)
			usleep(10);
		return (1);
	}
	sem_wait(philo->table->forks);
	if (philo->table->smb_died)
	{
		sem_post(philo->table->steward);
		sem_post(philo->table->forks);
		return (1);
	}
	msg(philo, "has taken a fork\n");
	sem_post(philo->table->steward);
	return (0);
}

static void		take_forks(t_philo *philo)
{
	if (forks_handler(philo))
		return ;
	if (philo->table->smb_died)
	{
		sem_post(philo->table->forks);
		sem_post(philo->table->forks);
		return ;
	}
	msg(philo, "is eating\n");
	philo->eat_num--;
	sem_wait(philo->table->time_sem);
	philo->last_lunch_t = take_time_in_ms();
	sem_post(philo->table->time_sem);
	my_wait(philo->table->eat_time);
	sem_post(philo->table->forks);
	sem_post(philo->table->forks);
}

static void		*check_die(void *val)
{
	t_philo	*philo;

	philo = (t_philo*)val;
	sem_wait(philo->table->time_sem);
	while (take_time_in_ms() - philo->last_lunch_t <= philo->table->die_time)
	{
		sem_post(philo->table->time_sem);
		usleep(10);
		sem_wait(philo->table->time_sem);
	}
	sem_post(philo->table->time_sem);
	if (!philo->eat_num)
		return (NULL);
	sem_wait(philo->table->death_sem);
	if (philo->table->smb_died)
	{
		sem_post(philo->table->death_sem);
		return (NULL);
	}
	msg(philo, "is died\n");
	philo->table->smb_died = 1;
	sem_post(philo->table->death_sem);
	return (NULL);
}

void			*simulation(void *val)
{
	t_philo		*philo;
	pthread_t	die_time_thrd;

	philo = (t_philo*)val;
	philo->sim_start = take_time_in_ms();
	philo->last_lunch_t = take_time_in_ms();
	pthread_create(&die_time_thrd, NULL, check_die, philo);
	while (philo->eat_num)
	{
		if (philo->table->smb_died)
			break ;
		take_forks(philo);
		if (philo->table->smb_died)
			break ;
		msg(philo, "is sleeping\n");
		my_wait(philo->table->sleep_time);
		if (philo->table->smb_died)
			break ;
		msg(philo, "is thinking\n");
	}
	pthread_join(die_time_thrd, NULL);
	return (NULL);
}

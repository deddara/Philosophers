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
	pthread_mutex_lock(&philo->table->steward);
	if (philo->table->smb_died)
	{
		pthread_mutex_unlock(&philo->table->steward);
		return (1);
	}
	sem_wait(philo->table->forks);
	msg(philo, "has taken a fork\n");
	if (philo->table->smb_died)
	{
		sem_post(philo->table->forks);
		pthread_mutex_unlock(&philo->table->steward);
		return (1);
	}
	sem_wait(philo->table->forks);
	msg(philo, "has taken a fork\n");
	pthread_mutex_unlock(&philo->table->steward);
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
	philo->last_lunch_t = take_time_in_ms();
	my_wait(philo->table->eat_time);
	sem_post(philo->table->forks);
	sem_post(philo->table->forks);
}

static void		*check_die(void *val)
{
	t_philo	*philo;
	int		time;

	philo = (t_philo*)val;
	time = take_time_in_ms();
	while (time - philo->last_lunch_t <= philo->table->die_time)
	{
		time = take_time_in_ms();
		usleep(10);
	}
	if (!philo->eat_num)
		return (NULL);
	pthread_mutex_lock(&philo->table->death_mutex);
	if (philo->table->smb_died)
	{
		pthread_mutex_unlock(&philo->table->death_mutex);
		return (NULL);
	}
	msg(philo, "is died\n");
	philo->table->smb_died = 1;
	pthread_mutex_unlock(&philo->table->death_mutex);
	return (NULL);
}

void			*simulation(void *val)
{
	t_philo		*philo;
	pthread_t	die_time_thrd;

	philo = (t_philo*)val;
	philo->table->sim_start = take_time_in_ms();
	philo->last_lunch_t = take_time_in_ms();
	pthread_create(&die_time_thrd, NULL, check_die, philo);
	while (philo->eat_num)
	{
		if (philo->table->smb_died)
			return (NULL);
		take_forks(philo);
		if (philo->table->smb_died)
			return (NULL);
		msg(philo, "is sleeping\n");
		my_wait(philo->table->sleep_time);
		if (philo->table->smb_died)
			return (NULL);
		msg(philo, "is thinking\n");
	}
	pthread_join(die_time_thrd, NULL);
	return (NULL);
}

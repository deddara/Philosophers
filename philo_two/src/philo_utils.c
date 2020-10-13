/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 18:19:43 by deddara           #+#    #+#             */
/*   Updated: 2020/10/12 18:19:44 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int			take_time_in_ms(void)
{
	struct timeval start;

	gettimeofday(&start, NULL);
	return ((int)((start.tv_sec) * 1000) + ((start.tv_usec) / 1000));
}

void		my_wait(int time)
{
	int				start;
	int				stop;
	int				time_diff;

	start = take_time_in_ms();
	stop = take_time_in_ms();
	time_diff = stop - start;
	while (time_diff < time)
	{
		stop = take_time_in_ms();
		time_diff = stop - start;
		usleep(10);
	}
}

void		msg(t_philo *philo, char *action)
{
	char	*time;
	char	*str;
	char	*id;
	int		new_time;

	sem_wait(philo->table->output);
	if (philo->table->smb_died)
	{
		sem_post(philo->table->output);
		return ;
	}
	new_time = take_time_in_ms() - philo->sim_start;
	time = ft_itoa(new_time);
	id = ft_itoa(philo->id + 1);
	str = ft_strjoin_philo(time, id, action);
	write(1, str, ft_strlen(str));
	free(id);
	free(time);
	free(str);
	sem_post(philo->table->output);
}

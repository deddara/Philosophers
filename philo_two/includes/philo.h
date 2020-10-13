/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 17:51:08 by deddara           #+#    #+#             */
/*   Updated: 2020/10/12 17:51:09 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <semaphore.h>

typedef struct		s_table
{
	int				phl_num;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				eat_num;
	int				smb_died;

	sem_t			*death_sem;
	sem_t			*steward;
	sem_t			*forks;
}					t_table;

typedef struct		s_philo
{
	int				id;
	int				last_lunch_t;
	int				eat_num;
	int				sim_start;
	t_table			*table;
}					t_philo;

void				ft_putstr_fd(char *s, int fd);
int					check_for_num(char *str);
int					ft_atoi(const char *str);
char				*ft_itoa(int n);
char				*ft_strjoin_philo(char *time, char *id, char *action);
int					ft_strlen(char *str);
int					init_and_threads(t_table *table, char **argv);
void				*simulation(void *val);
void				msg(t_philo *philo, char *action);
void				my_wait(int time);
int					take_time_in_ms(void);

#endif

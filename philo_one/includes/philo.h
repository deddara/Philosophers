#ifndef PHILO_H
# define PHILO_H

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct	s_table
{
				int 			phl_num;
				int 			die_time;
				int 			eat_time;
				int 			sleep_time;
				int 			eat_num;
				pthread_mutex_t	*forks;
}				t_table;

typedef struct	s_philo
{
				int		id;
				t_table	*table;
}				t_philo;


void	ft_putstr_fd(char *s, int fd);
int		check_for_num(char *str);
int				ft_atoi(const char *str);
#endif
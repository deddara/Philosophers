#ifndef PHILO_H
# define PHILO_H

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct	s_table
{
				int i;
				pthread_mutex_t *forks;
}				t_table;

void	ft_putstr_fd(char *s, int fd);
int		check_for_num(char *str);
#endif
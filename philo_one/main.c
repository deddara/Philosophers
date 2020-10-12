#include <unistd.h>
#include <pthread.h>
#include "philo.h"
#include <stdio.h>
#include <sys/time.h>

static int take_time_in_ms(void)
{
	struct timeval start;

	gettimeofday(&start, NULL);
	return ((int)((start.tv_sec) * 1000) + ((start.tv_usec) / 1000));
}

static void my_wait(int time)
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

static void msg(t_philo *philo, char *action)
{
	char *time;
	char *str;
	char *id;
	int new_time;

	new_time = take_time_in_ms() - philo->table->sim_start;
	time = ft_itoa(new_time);
	id = ft_itoa(philo->id);
	str = ft_strjoin_philo(time, id, action);
	write(1, str, ft_strlen(str));
	free(id);
	free(time);
	free(str);
}

static int forks_handler(t_philo *philo, int first_fork, int second_fork)
{
	pthread_mutex_lock(&philo->table->forks[first_fork]);
	if (philo->table->smb_died)
	{
		pthread_mutex_unlock(&philo->table->forks[first_fork]);
		return (1);
	}
	msg(philo, "has taken a fork\n");
	pthread_mutex_lock(&philo->table->forks[second_fork]);
	if (philo->table->smb_died)
	{
		pthread_mutex_unlock(&philo->table->forks[first_fork]);
		pthread_mutex_unlock(&philo->table->forks[second_fork]);
		return (1);
	}
	msg(philo, "has taken a fork\n");
	return (0);
}

static void take_forks(t_philo *philo)
{
	int right_fork;

	right_fork = ((philo->id + 1) % philo->table->phl_num);
	if (philo->id % 2 == 0)
	{
		if (forks_handler(philo, philo->id, right_fork))
			return ;
	}
	else
	{
		if (forks_handler(philo, right_fork, philo->id))
			return ;
	}
	if (philo->table->smb_died)
	{
		pthread_mutex_unlock(&philo->table->forks[philo->id]);
		pthread_mutex_unlock(&philo->table->forks[right_fork]);
		return ;
	}
	msg(philo, "is eating\n");
	philo->eat_num--;
	philo->last_lunch_t = take_time_in_ms();
	my_wait(philo->table->eat_time);
	pthread_mutex_unlock(&philo->table->forks[philo->id]);
	pthread_mutex_unlock(&philo->table->forks[right_fork]);
}



static void *check_die(void *val)
{
	t_philo *philo;
	int time;

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

static void ft_sleep(t_philo *philo)
{
	msg(philo, "is sleeping\n");
	my_wait(philo->table->sleep_time);
}


static void *simulation(void *val)
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
		ft_sleep(philo);
		if (philo->table->smb_died)
			return (NULL);
		msg(philo, "is thinking\n");
	}
	pthread_join(die_time_thrd, NULL);
	return (NULL);
}

static void init_philo(int i, t_philo *philo, t_table *table)
{
	philo->id = i;
	philo->table = table;
	philo->eat_num = table->eat_num;
	philo->is_died = 0;
}

static void start_threads(t_table *table, pthread_mutex_t *forks)
{
	int i;
	t_philo philo[table->phl_num];
	pthread_t thread[table->phl_num];

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

static void init_mutex(t_table *table)
{
	int						i;
	pthread_mutex_t			forks[table->phl_num];
	static pthread_mutex_t	death_mutex = PTHREAD_MUTEX_INITIALIZER;

	i = 0;
	while (i < table->phl_num)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	table->forks = forks;
	table->death_mutex = death_mutex;
	start_threads(table, forks);
	pthread_mutex_destroy(&death_mutex);
}

static int init(t_table *table, char **argv)
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
	table->start = 0;
	table->smb_died = 0;
	init_mutex(table);
	return (0);
}

int check_validation(int argc, char **argv)
{
	int i;

	i = 1;
	if (argc < 5 || argc > 6)
	{
		ft_putstr_fd("invalid number of arguments\n", 2);
		return (1);
	}
	while (argv[i])
	{
		if (check_for_num(argv[i]))
		{
			ft_putstr_fd("wrong number: ", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd("\n", 2);
			return (1);
		}
		i++;
	}
	return (0);
}

int main(int argc, char **argv)
{
	t_table		table;
	if (check_validation(argc, argv))
		return (1);
	if (init(&table, argv))
		return(1);
	return (0);
}
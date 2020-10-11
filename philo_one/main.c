#include <unistd.h>
#include <pthread.h>
#include "philo.h"
#include <stdio.h>
#include <sys/time.h>
//void  *philo(void *val)
//{
//	t_table *table = (t_table *)val;
//	pthread_mutex_lock(&table->l_f);
//	write(1, "s\n", 2);
//	pthread_mutex_unlock(&table->l_f);
//	return NULL;
//}

static int take_time_in_ms(void)
{
	struct timeval start;

	gettimeofday(&start, NULL);
	return (((start.tv_sec) * 1000) + ((start.tv_usec)/1000));
}

static void take_forks(t_philo *philo)
{
	int left_fork;
	int right_fork;

	left_fork = (philo->id - 1 + philo->table->phl_num) % philo->table->phl_num;
	right_fork = ((philo->id + 1) % philo->table->phl_num);
	if (!philo->id % 2)
	{
		pthread_mutex_lock(&philo->table->forks[left_fork]);
		ft_putnbr_fd(take_time_in_ms(), 1);
		ft_putnbr_fd(philo->id, 1);
		ft_putstr_fd("has taken a fork\n", 1);
		pthread_mutex_lock(&philo->table->forks[right_fork]);
		ft_putnbr_fd(take_time_in_ms(), 1);
		ft_putnbr_fd(philo->id, 1);
		ft_putstr_fd("has taken a fork\n", 1);
	}
	else
	{
		pthread_mutex_lock(&philo->table->forks[right_fork]);
		ft_putnbr_fd(take_time_in_ms(), 1);
		ft_putnbr_fd(philo->id, 1);
		ft_putstr_fd("has taken a fork\n", 1);
		pthread_mutex_lock(&philo->table->forks[left_fork]);
		ft_putnbr_fd(take_time_in_ms(), 1);
		ft_putnbr_fd(philo->id, 1);
		ft_putstr_fd("has taken a fork\n", 1);
	}
	ft_putnbr_fd(take_time_in_ms(), 1);
	ft_putstr_fd(" ", 1);
	ft_putnbr_fd(philo->id, 1);
	ft_putstr_fd(" is eating\n", 1);
	usleep(philo->table->eat_time * 1000);
	philo->last_lunch_t = take_time_in_ms();
	pthread_mutex_unlock(&philo->table->forks[left_fork]);
	pthread_mutex_unlock(&philo->table->forks[right_fork]);

}



static void *check_die(void *val)
{
	t_philo *philo = (t_philo*)val;
	int time;

	time = take_time_in_ms();
	while (time - philo->last_lunch_t < philo->table->die_time)
	{
		usleep(10000);
	}
	ft_putnbr_fd(take_time_in_ms(), 1);
	ft_putstr_fd(" ", 1);
	ft_putnbr_fd(philo->id, 1);
	ft_putstr_fd(" died\n", 1);
	philo->table->smb_died = 1;
	return (NULL);
}

static void ft_sleep(t_philo *philo)
{
	ft_putnbr_fd(take_time_in_ms(), 1);
	ft_putstr_fd(" ", 1);
	ft_putnbr_fd(philo->id, 1);
	ft_putstr_fd(" is sleeping\n", 1);
	usleep(philo->table->sleep_time * 1000);
}

static void ft_think(t_philo *philo)
{
	ft_putnbr_fd(take_time_in_ms(), 1);
	ft_putstr_fd(" ", 1);
	ft_putnbr_fd(philo->id, 1);
	ft_putstr_fd(" is thinking\n", 1);
}

static void *simulation(void *val)
{
	t_philo		*philo;
	pthread_t	die_time_thrd;

	philo = (t_philo*)val;
	pthread_create(&die_time_thrd, NULL, check_die, philo);
	philo->last_lunch_t = take_time_in_ms();
	while (philo->table->eat_num)
	{
		if (philo->table->smb_died)
			return (NULL);
		take_forks(philo);
		if (philo->table->smb_died)
			return (NULL);
		ft_sleep(philo);
		if (philo->table->smb_died)
			return (NULL);
		ft_think(philo);
	}
	return (NULL);
}

static void init_mutex(t_table *table)
{
	int i;
	pthread_mutex_t	forks[table->phl_num];

	i = 0;
	while (i < table->phl_num)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	table->forks = forks;
}

static void init_philo(int i, t_philo *philo, t_table *table)
{
	philo->id = i;
	philo->table = table;
	philo->is_died = 0;
}

static void start_threads(t_table *table)
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
	start_threads(table);
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
//	pthread_t	thread;
//	pthread_t	thread1;
	if (check_validation(argc, argv))
		return (1);
	if (init(&table, argv))
		return(1);

//	pthread_mutex_init(&table.l_f, NULL);
//	pthread_create(&thread, NULL, philo, &table);
//	pthread_create(&thread1, NULL, philo, &table);

//	pthread_join(thread, NULL);


	return (0);
}
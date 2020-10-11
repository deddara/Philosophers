#include <unistd.h>
#include <pthread.h>
#include "philo.h"
#include <stdio.h>
//void  *philo(void *val)
//{
//	t_table *table = (t_table *)val;
//	pthread_mutex_lock(&table->l_f);
//	write(1, "s\n", 2);
//	pthread_mutex_unlock(&table->l_f);
//	return NULL;
//}

static void *check_die(void *val)
{
	t_philo *philo = (t_philo*)val;
	return (NULL);
}

static void *simulation(void *val)
{
	t_philo *philo = (t_philo*)val;
	pthread_t	die_time_thrd;

	pthread_create(&die_time_thrd, NULL, check_die, philo);
	printf("%d\n", philo->id);
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